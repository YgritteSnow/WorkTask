#ifndef __BRUSH_H__
#define __BRUSH_H__

#include <memory.h>
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

		if (ShaderManager::TestPixel(v1)) {
			return;
		}
		byte pixel[VERTEX_MAX_SIZE];
		ShaderManager::ProcessPixel(v1, pixel);
		back_buffer->SetPixelAt_byVertex(coord_x, coord_y, vid, pixel);
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

		float ratio;
		for (int idx = 0; idx < max_count; ++idx) {
			ratio = use_x ? _calRat(v0pos._x, v1pos._x, vpos._x) : _calRat(v0pos._y, v1pos._y, vpos._y);
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
		auto& v0pos = *static_cast<WorldPos*>(static_cast<void*>(v0 + offset_pos));
		auto& v1pos = *static_cast<WorldPos*>(static_cast<void*>(v1 + offset_pos));

		if (JMath::f_equal(v0pos._y, v1pos._y)) {
			return;
		}

		if (v0pos._x > v1pos._x) {
			std::swap(v0, v1);
			std::swap(v0pos, v1pos);
		}

		byte v[VERTEX_MAX_SIZE];
		auto& vpos = *static_cast<WorldPos*>(static_cast<void*>(v + offset_pos));
		memcpy(v, v0, byte_size);
		vpos._y = static_cast<float>(y);

		float ratio;
		do {
			ratio = _calRat(v0pos._y, v1pos._y, y);
			_interp(ratio, v0, v1, v, byte_size, offset_pos);
			DrawDot(vid, v, back_buffer);
			vpos._x += 1;
		} while (vpos._x <= v1pos._x);
	}

	template <typename _TexBuffer>
	static void DrawTriangle(
		STRUCT_ID vid,
		byte* v0, byte* v1, byte* v2,
		_TexBuffer* back_buffer
	) {
		STRIDE_TYPE byte_size = StructReflectManager::GetOffsetSize(vid);

		const auto& offset_pos = StructReflectManager::GetOffset<POSITION>(vid, 0);
		auto& v0pos = *static_cast<WorldPos*>(static_cast<void*>(v0 + offset_pos));
		auto& v1pos = *static_cast<WorldPos*>(static_cast<void*>(v1 + offset_pos));
		auto& v2pos = *static_cast<WorldPos*>(static_cast<void*>(v2 + offset_pos));

		// 转换使三个点为y值从小到大排列
		if (v0pos._y > v1pos._y) {
			std::swap(v0, v1);
			std::swap(v0pos, v1pos);
		}
		if (v0pos._y > v2pos._y) {
			std::swap(v0, v2);
			std::swap(v0pos, v2pos);
		}
		if (v1pos._y > v2pos._y) {
			std::swap(v1, v2);
			std::swap(v1pos, v2pos);
		}

		// 整数格子
		ScreenCoord y0_i = static_cast<ScreenCoord>(v0pos._y);
		ScreenCoord y1_i = static_cast<ScreenCoord>(v1pos._y);
		ScreenCoord y2_i = static_cast<ScreenCoord>(v2pos._y);
		float y0 = v0pos._y;
		float y1 = v1pos._y;

		// 找到分割的中间点
		if (JMath::f_equal(v0pos._y, v2pos._y)) {
			return;
		}

		byte v_left[VERTEX_MAX_SIZE];
		byte v_right[VERTEX_MAX_SIZE];
		auto& v_left_pos = *static_cast<WorldPos*>(static_cast<void*>(v_left + offset_pos));
		auto& v_right_pos = *static_cast<WorldPos*>(static_cast<void*>(v_right + offset_pos));

		// 对x和y进行线性步进
		v_left_pos._x = v0pos._x;
		v_right_pos._x = v0pos._x;
		float k02 = (v2pos._x - v0pos._x) / (v2pos._y - v0pos._y);
		float k01 = (v1pos._x - v0pos._x) / (v1pos._y - v0pos._y);
		float k12 = (v1pos._x - v2pos._x) / (v1pos._y - v2pos._y);

		float ratio_left, ratio_right;
		if (!JMath::f_equal(v0pos._y, v1pos._y)) {
			while (y0_i <= y1_i) {
				v_left_pos._y = y0;
				v_right_pos._y = y0;
				ratio_left = _calRat(v0pos._y, v2pos._y, y0);
				ratio_right = _calRat(v0pos._y, v1pos._y, y0);
				_interp(ratio_left, v0, v2, v_left, byte_size, offset_pos);
				_interp(ratio_right, v0, v1, v_right, byte_size, offset_pos);
				DrawLine_h(vid, v_left, v_right, y0_i, back_buffer);
				v_left_pos._x += k02;
				v_right_pos._x += k01;
				++y0_i;
				++y0;
			}
		}
		if (!JMath::f_equal(v1pos._y, v2pos._y)) {
			while (y1_i < y2_i) {
				v_left_pos._y = y1;
				v_right_pos._y = y1;
				ratio_left = _calRat(v0pos._y, v2pos._y, y1);
				ratio_right = _calRat(v1pos._y, v2pos._y, y1);
				_interp(ratio_left, v0, v2, v_left, byte_size, offset_pos);
				_interp(ratio_right, v1, v2, v_right, byte_size, offset_pos);
				DrawLine_h(vid, v_left, v_right, y1_i, back_buffer);
				v_left_pos._x += k02;
				v_right_pos._x += k12;
				++y1_i;
				++y1;
			}
		}
	}

	/* *********************************************
	* 工具函数
	* *********************************************/
	static void _correctPerpectUV(const float z_start, const float z_end, float z_cur,
								  const float t_start, const float t_end, float& t_cur) {
		float ratio = _calRat(z_start, z_end, z_cur);
		t_cur = t_end + (t_start - t_end) * ratio;
	}

	static void _interp(const float ratio, const byte* t_start, const byte* t_end, byte* t_cur, STRIDE_TYPE byte_size, STRIDE_TYPE pos_stride) {
		for (STRIDE_TYPE i = 0; i < byte_size; i+=sizeof(float)) {
			if (i == pos_stride) {
				i += sizeof(float) * 2; // 定位到z的位置
				GetFloat(t_cur, i) = GetFloat(t_start, i) + (GetFloat(t_end, i) - GetFloat(t_start, i)) * ratio;
			}
			else {
				GetFloat(t_cur, i) = GetFloat(t_start, i) + (GetFloat(t_end, i) - GetFloat(t_start, i)) * ratio;
			}
		}
	}
	static void _interp(const float ratio, const float t_start, const float t_end, float& t_cur) {
		t_cur = t_start + (t_end - t_start) * ratio;
	}

	static float _calRat(float start, float end, float cur) {
#if USE_PERSPECTIVE_CORRECT
		return JMath::f_equal(1/start, 1/end) ? 
			(cur - end) / (start - end)
			: ((1 / cur - 1 / end) / (1 / start - 1 / end));
#else
		return (cur - end) / (start - end);
#endif
	}
};

#endif