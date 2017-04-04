#ifndef __BRUSH_H__
#define __BRUSH_H__

#include <memory.h>
#include <assert.h>
#include "JMath.h"
#include "Color.h"
#include "TexManager.h"
#include "ABuffer.h"
#include "StructReflection.h"
#include "ShaderManager.h"

// 顶点最大size
// pos(3)+color(4)+uv(2)+normal(3)=(float)12=48
// 考虑到G-buffer可能会用的比较多，所以这里暂定128
#define VERTEX_MAX_SIZE 128

#define USE_PERSPECTIVE_CORRECT 1
inline
const float& GetFloat(const byte* x, STRUCT_ID offset) {
	return *static_cast<const float*>(static_cast<const void*>((x)+(offset)));
}

inline
float& GetFloat(byte* x, STRUCT_ID offset) {
	return *static_cast<float*>(static_cast<void*>((x)+(offset)));
}

// 使用顶点数据进行画图
class VertexBrush {
public:
	template <typename ColorType>
	static void DrawDot(
		STRUCT_ID vid,
		byte* v1,
		TexBuffer<ColorType>* back_buffer
	) {
		const auto& v1pos = *static_cast<WorldPos*>(static_cast<void*>(v1 + StructReflectManager::GetOffset<POSITION>(vid, 0)));

		ScreenCoord coord_x = static_cast<ScreenCoord>(v1pos._x);
		ScreenCoord coord_y = static_cast<ScreenCoord>(v1pos._y);

		if (!ShaderManager::TestPixel(v1)) {
			return;
		}
		byte pixel[VERTEX_MAX_SIZE];
		ShaderManager::ProcessPixel(v1, pixel);
		back_buffer->SetPixelAt_byVertex(coord_x, coord_y, StructWrapper(vid, pixel));
	}

	template <typename _TexBuffer>
	static void DrawLine(
		STRUCT_ID vid,
		byte* v0, byte* v1,
		_TexBuffer* back_buffer
	) {
		STRIDE_TYPE byte_size = StructReflectManager::GetOffsetSize(vid);
		const auto& offset_pos = StructReflectManager::GetOffset<POSITION>(vid, 0);
		const auto& v0pos = *static_cast<WorldPos*>(static_cast<void*>(v0 + offset_pos));
		const auto& v1pos = *static_cast<WorldPos*>(static_cast<void*>(v1 + offset_pos));

		if (JMath::f_equal(v1pos._x, v0pos._x) && JMath::f_equal(v1pos._y, v0pos._y)) {
			return;
		}

		byte v[VERTEX_MAX_SIZE];
		auto& vpos = *static_cast<WorldPos*>(static_cast<void*>(v + offset_pos));

		// 步进长度
		float x_step = v1pos._x - v0pos._x;
		float y_step = v1pos._y - v0pos._y;
		bool use_x = fabs(x_step) > fabs(y_step);
		float max_count = max(fabs(x_step), fabs(y_step));
		x_step /= max_count;
		y_step /= max_count;

		vpos._x = v0pos._x;
		vpos._y = v0pos._y;

		float ratio;
		for (int idx = 0; idx < max_count; ++idx) {
			ratio = use_x ? calRat(v0pos._x, v1pos._x, vpos._x, v0pos._z, v1pos._z)
				: calRat(v0pos._y, v1pos._y, vpos._y, v0pos._z, v1pos._z);
			_interp(ratio, v0, v1, v, byte_size, offset_pos);

			DrawDot(vid, v, back_buffer);
			vpos._y += y_step;
			vpos._x += x_step;
		}
	}

	template <typename _TexBuffer>
	static void DrawLine_h(
		STRUCT_ID vid,
		byte* v0, byte* v1,
		ScreenCoord y,
		_TexBuffer* back_buffer
	) {
		STRIDE_TYPE byte_size = StructReflectManager::GetOffsetSize(vid);

		const auto& offset_pos = StructReflectManager::GetOffset<POSITION>(vid, 0);
		auto v0pos = *static_cast<WorldPos*>(static_cast<void*>(v0 + offset_pos));
		auto v1pos = *static_cast<WorldPos*>(static_cast<void*>(v1 + offset_pos));

		if (v0pos._x > v1pos._x) {
			std::swap(v0, v1);
		}
		v0pos = *static_cast<WorldPos*>(static_cast<void*>(v0 + offset_pos));
		v1pos = *static_cast<WorldPos*>(static_cast<void*>(v1 + offset_pos));

		byte v[VERTEX_MAX_SIZE];
		auto& vpos = *static_cast<WorldPos*>(static_cast<void*>(v + offset_pos));
		vpos._y = static_cast<float>(y);
		ScreenCoord bgn = static_cast<ScreenCoord>(v0pos._x+0.5);
		ScreenCoord end = static_cast<ScreenCoord>(v1pos._x+0.5);
		vpos._x = bgn;
		float ratio;
		do {
			ratio = calRat(v0pos._x, v1pos._x, vpos._x, v0pos._z, v1pos._z);
			_interp(ratio, v0, v1, v, byte_size, offset_pos);
			DrawDot(vid, v, back_buffer);
			vpos._x += 1;
			bgn += 1;
		} while (bgn < end);
	}

	template <typename _TexBuffer>
	static void DrawTriangle(
		STRUCT_ID vid,
		byte* v0, byte* v1, byte* v2,
		_TexBuffer* back_buffer
	) {
		STRIDE_TYPE byte_size = StructReflectManager::GetOffsetSize(vid);

		const auto& offset_pos = StructReflectManager::GetOffset<POSITION>(vid, 0);
		auto v0pos = *static_cast<WorldPos*>(static_cast<void*>(v0 + offset_pos));
		auto v1pos = *static_cast<WorldPos*>(static_cast<void*>(v1 + offset_pos));
		auto v2pos = *static_cast<WorldPos*>(static_cast<void*>(v2 + offset_pos));

		// 转换使三个点为y值从小到大排列
		if (v0pos._y > v1pos._y) {
			std::swap(v0, v1);
		}
		if (v0pos._y > v2pos._y) {
			std::swap(v0, v2);
		}
		if (v1pos._y > v2pos._y) {
			std::swap(v1, v2);
		}
		v0pos = *static_cast<WorldPos*>(static_cast<void*>(v0 + offset_pos));
		v1pos = *static_cast<WorldPos*>(static_cast<void*>(v1 + offset_pos));
		v2pos = *static_cast<WorldPos*>(static_cast<void*>(v2 + offset_pos));

		// 整数格子
		ScreenCoord y0_i = static_cast<ScreenCoord>(v0pos._y);
		ScreenCoord y1_i = static_cast<ScreenCoord>(v1pos._y);
		ScreenCoord y2_i = static_cast<ScreenCoord>(v2pos._y);

		// 找到分割的中间点
		if(y0_i == y2_i){
		//if (JMath::f_equal(v0pos._y, v2pos._y)) {
			return;
		}

		byte v_left[VERTEX_MAX_SIZE];
		byte v_right[VERTEX_MAX_SIZE];
		auto& v_left_pos = *static_cast<WorldPos*>(static_cast<void*>(v_left + offset_pos));
		auto& v_right_pos = *static_cast<WorldPos*>(static_cast<void*>(v_right + offset_pos));

		// 修正顶点
		float slide_02 = (v2pos._x - v0pos._x) / (v2pos._y - v0pos._y);
		float slide_01 = (v1pos._x - v0pos._x) / (v1pos._y - v0pos._y);
		float slide_12 = (v1pos._x - v2pos._x) / (v1pos._y - v2pos._y);
		float x_02_0 = v0pos._x + (y0_i - v0pos._y)*slide_02;
		float x_01_0 = v0pos._x + (y0_i - v0pos._y)*slide_01;
		float x_01_1 = v0pos._x + (y1_i - v0pos._y)*slide_01;
		float x_12_1 = v1pos._x + (y1_i - v1pos._y)*slide_12;
		float x_02_2 = v2pos._x + (y2_i - v2pos._y)*slide_02;
		float x_12_2 = v2pos._x + (y2_i - v2pos._y)*slide_12;
		float x_02_1 = v2pos._x + (y1_i - v2pos._y)*slide_02;

		// 对x和y进行线性步进
		v_left_pos._x = x_02_0;
		v_right_pos._x = x_01_0;
		float k02 = (x_02_2 - x_02_0) / (y2_i - y0_i);
		float k01 = (x_01_1 - x_01_0) / (y1_i - y0_i);
		float k12 = (x_12_1 - x_12_2) / (y1_i - y2_i);

		float ratio_left, ratio_right;
		if (y0_i != y1_i) {
			while (y0_i < y1_i) {
				v_left_pos._y = y0_i;
				v_right_pos._y = y0_i;
				ratio_left = calRat(v0pos._y, v2pos._y, y0_i, v0pos._z, v2pos._z);
				ratio_right = calRat(v0pos._y, v1pos._y, y0_i, v0pos._z, v1pos._z);
				_interp(ratio_left, v0, v2, v_left, byte_size, offset_pos);
				_interp(ratio_right, v0, v1, v_right, byte_size, offset_pos);
				DrawLine_h(vid, v_left, v_right, y0_i, back_buffer);
				v_left_pos._x += k02;
				v_right_pos._x += k01;
				++y0_i;
			}
		}

		v_left_pos._x = x_02_1;
		v_right_pos._x = x_12_1;
		if (y1_i != y2_i) {
			while (y1_i < y2_i) {
				v_left_pos._y = y1_i;
				v_right_pos._y = y1_i;
				ratio_left = calRat(v0pos._y, v2pos._y, y1_i, v0pos._z, v2pos._z);
				ratio_right = calRat(v1pos._y, v2pos._y, y1_i, v1pos._z, v2pos._z);
				_interp(ratio_left, v0, v2, v_left, byte_size, offset_pos);
				_interp(ratio_right, v1, v2, v_right, byte_size, offset_pos);
				DrawLine_h(vid, v_left, v_right, y1_i, back_buffer);
				v_left_pos._x += k02;
				v_right_pos._x += k12;
				++y1_i;
			}
		}
	}

	/* *********************************************
	* 工具函数
	* *********************************************/
	static void _interp(const float ratio, const byte* t_start, const byte* t_end, byte* t_cur, STRIDE_TYPE byte_size, STRIDE_TYPE pos_stride) {
		for (STRIDE_TYPE i = 0; i < byte_size; i+=sizeof(float)) {
			if (i == pos_stride) {
				i += sizeof(float) * 2; // 定位到z的位置
				GetFloat(t_cur, i) = GetFloat(t_end, i) + (GetFloat(t_start, i) - GetFloat(t_end, i)) * ratio;
			}
			else {
				GetFloat(t_cur, i) = GetFloat(t_end, i) + (GetFloat(t_start, i) - GetFloat(t_end, i)) * ratio;
			}
		}
	}
	static void _interp(const float ratio, const float t_start, const float t_end, float& t_cur) {
		t_cur = t_end + (t_start - t_end) * ratio;
	}

	static float calRat(float x_start, float x_end, float x_cur, float z_start, float z_end) {
		float linear_ratio = _calRat(x_start, x_end, x_cur);
#if USE_PERSPECTIVE_CORRECT
		float new_ratio = _transPerspRat(linear_ratio, z_start, z_end);
		return new_ratio;
#else
		return linear_ratio;
#endif
	}
	inline
	static float _calRat(float start, float end, float cur) {
		return JMath::f_equal(start, end) ? 0 : (cur - end) / (start - end);
	}
	static float _transPerspRat(float ratio, float z_start, float z_end) {
		float z_cur;
		if (JMath::f_equal(z_start, 0)) {
			return ratio;// assert(0);
		}
		else if (JMath::f_equal(z_end, 0)) {
			return ratio;// assert(0);
		}
		else if (JMath::f_equal(z_start, z_end)) {
			return ratio;
		}

		z_cur = 1 / ((1 / z_start - 1 / z_end) * ratio + 1 / z_end);
		return _calRat(z_start, z_end, z_cur);
	}
};

#endif