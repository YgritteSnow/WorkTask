#ifndef __BRUSH_H__
#define __BRUSH_H__

#include <memory.h>
#include "JMath.h"
#include "Color.h"
#include "Texture.h"
#include "TargetBuffer.h"

// 使用顶点数据进行画图
template <typename VertexStruct>
class VertexBrush {
public:
	template <typename TargetBuffer>
	static void DrawDot(
		VertexStruct v1,
		TargetBuffer* back_buffer,
		ImgBuffer<DepthBufferPixel>* depth_buffer
	) {

		ScreenCoord coord_x = static_cast<ScreenCoord>(v1.pos._x);
		ScreenCoord coord_y = static_cast<ScreenCoord>(v1.pos._y);

		// 排除近平面以前、远平面以后、相机背面
		if (v1.pos._z < 0 || v1.pos._z > 1) {
			return;
		}
		// 排除屏幕以外
		else if (coord_x < 0 || coord_x >= back_buffer->width || coord_y < 0 || coord_y >= back_buffer->height) {
			return;
		}
		// 进行深度测试
		else if (depth_buffer && v1.pos._z >= depth_buffer->GetPixel_coordPos(coord_x, coord_y)) {
			return;
		}

		// 更新深度缓存
		if ( depth_buffer && v1.pos._z < depth_buffer->GetPixel_coordPos(coord_x, coord_y)) {
			*depth_buffer->pPixelAt(coord_x, coord_y) = v1.pos._z;
		}
		// 根据贴图更新颜色
		if (TextureManager::GetInstance()->GetTexture()) {
			v1.color *= TextureManager::GetInstance()->GetTexture()->GetPixel_normedPos_smart(v1.uv._x, v1.uv._y);
		}

		//back_buffer->SetPixelAt(coord_x, coord_y, v1.color);
		back_buffer->SetPixelAt_byVertex(coord_x, coord_y, v1);
	}

	template <typename TargetBuffer>
	static void DrawLine(
		VertexStruct v0, VertexStruct v1,
		TargetBuffer* back_buffer,
		ImgBuffer<DepthBufferPixel>* depth_buffer
	) {
		if (JMath::f_equal(v1.pos._x, v0.pos._x) && JMath::f_equal(v1.pos._y, v0.pos._y)) {
			return;
		}

		VertexStruct v = v0;

		// 步进长度
		float x_step = v1.pos._x - v0.pos._x;
		float y_step = v1.pos._y - v0.pos._y;
		bool use_x = fabs(x_step) > fabs(y_step);
		float max_count = max(fabs(x_step), fabs(y_step));
		x_step /= max_count;
		y_step /= max_count;
		float z_step = (v1.pos._z - v0.pos._z) / max_count;

		// 颜色插值
		NormColor4 color_bgn = v0.color;
		NormColor4 color_step = (v1.color - v0.color) / max_count;

		for (int idx = 0; idx < max_count; ++idx) {
			float defaultRat = use_x ? _calRat(v0.pos._x, v1.pos._x, v.pos._x) : _calRat(v0.pos._y, v1.pos._y, v.pos._y);
			_correctPerpectUV(v0.pos._z, v1.pos._z, v.pos._z, v0.uv, v1.uv, v.uv, defaultRat);
			DrawDot(v, back_buffer, depth_buffer);
			v.color += color_step;
			v.pos._z += z_step;
			v.pos._y += y_step;
			v.pos._x += x_step;
		}
	}

	template <typename TargetBuffer>
	static void DrawLine_h(
		VertexStruct v0, VertexStruct v1,
		ScreenCoord y,
		TargetBuffer* back_buffer,
		ImgBuffer<DepthBufferPixel>* depth_buffer
	) {
		if (v0.pos._x > v1.pos._x) {
			std::swap(v0, v1);
		}

		VertexStruct v = v0;
		v.pos._y = static_cast<float>(y);

		NormColor4 color_step = JMath::f_equal(v1.pos._x, v0.pos._x) ? NormColor4() : ((v1.color - v0.color) / (v1.pos._x - v0.pos._x));
		float z_step = JMath::f_equal(v1.pos._x, v0.pos._x) ? 0 : ((v1.pos._z - v0.pos._z) / (v1.pos._x - v0.pos._x));
		do {
			_correctPerpectUV(v0.pos._z, v1.pos._z, v.pos._z, v0.uv, v1.uv, v.uv, _calRat(v0.pos._x, v1.pos._x, v.pos._x));
			DrawDot(v, back_buffer, depth_buffer);
			v.color += color_step;
			v.pos._z += z_step;
			v.pos._x += 1;
		} while (v.pos._x <= v1.pos._x);
	}

	template <typename TargetBuffer>
	static void DrawTriangle(
		VertexStruct v0, VertexStruct v1, VertexStruct v2,
		TargetBuffer* back_buffer,
		ImgBuffer<DepthBufferPixel>* depth_buffer
	) {
		// 转换使三个点为y值从小到大排列
		if (v0.pos._y > v1.pos._y) {
			std::swap(v0, v1);
		}
		if (v0.pos._y > v2.pos._y) {
			std::swap(v0, v2);
		}
		if (v1.pos._y > v2.pos._y) {
			std::swap(v1, v2);
		}

		// 整数格子
		ScreenCoord y0_i = static_cast<ScreenCoord>(v0.pos._y);
		ScreenCoord y1_i = static_cast<ScreenCoord>(v1.pos._y);
		ScreenCoord y2_i = static_cast<ScreenCoord>(v2.pos._y);
		float y0 = v0.pos._y;
		float y1 = v1.pos._y;

		// 找到分割的中间点
		if (JMath::f_equal(v0.pos._y, v2.pos._y)) {
			return;
		}

		VertexStruct v_left;
		VertexStruct v_right;

		// 对位置插值
		v_left.pos._x = v0.pos._x;
		v_right.pos._x = v0.pos._x;
		float k02 = (v2.pos._x - v0.pos._x) / (v2.pos._y - v0.pos._y);
		float k01 = (v1.pos._x - v0.pos._x) / (v1.pos._y - v0.pos._y);
		float k12 = (v1.pos._x - v2.pos._x) / (v1.pos._y - v2.pos._y);

		// 对 1/z 的插值
		v_left.pos._z = v0.pos._z;
		v_right.pos._z = v0.pos._z;
		float zk02 = (1.f / (v2.pos._y - v0.pos._y)) * (v2.pos._z - v0.pos._z);
		float zk01 = (1.f / (v1.pos._y - v0.pos._y)) * (v1.pos._z - v0.pos._z);
		float zk12 = (1.f / (v2.pos._y - v1.pos._y)) * (v2.pos._z - v1.pos._z);

		// 对颜色插值
		v_left.color = v0.color;
		v_right.color = v0.color;
		NormColor4 color02 = (1.f / (v2.pos._y - v0.pos._y)) * (v2.color - v0.color);
		NormColor4 color01 = (1.f / (v1.pos._y - v0.pos._y)) * (v1.color - v0.color);
		NormColor4 color12 = (1.f / (v2.pos._y - v1.pos._y)) * (v2.color - v1.color);

		if (!JMath::f_equal(v0.pos._y, v1.pos._y)) {
			while (y0_i < y1_i) {
				_correctPerpectUV(v0.pos._z, v2.pos._z, v_left.pos._z, v0.uv, v2.uv, v_left.uv, _calRat(v0.pos._y, v2.pos._y, y0));
				_correctPerpectUV(v0.pos._z, v1.pos._z, v_right.pos._z, v0.uv, v1.uv, v_right.uv, _calRat(v0.pos._y, v1.pos._y, y0));
				DrawLine_h(v_left, v_right, y0_i, back_buffer, depth_buffer);
				v_left.pos._x += k02;
				v_right.pos._x += k01;
				v_left.color += color02;
				v_right.color += color01;
				v_left.pos._z += zk02;
				v_right.pos._z += zk01;
				++y0_i;
				++y0;
			}

		}

		v_right.pos._x = v1.pos._x;
		v_right.color = v1.color;
		v_right.pos._z = v1.pos._z;

		if (!JMath::f_equal(v1.pos._y, v2.pos._y)) {
			while (y1_i < y2_i) {
				_correctPerpectUV(v0.pos._z, v2.pos._z, v_left.pos._z, v0.uv, v2.uv, v_left.uv, _calRat(v0.pos._y, v2.pos._y, y1));
				_correctPerpectUV(v1.pos._z, v2.pos._z, v_right.pos._z, v1.uv, v2.uv, v_right.uv, _calRat(v1.pos._y, v2.pos._y, y1));
				DrawLine_h(v_left, v_right, y1_i, back_buffer, depth_buffer);
				v_left.pos._x += k02;
				v_right.pos._x += k12;
				v_left.color += color02;
				v_right.color += color12;
				v_left.pos._z += zk02;
				v_right.pos._z += zk12;
				++y1_i;
				++y1;
			}
		}
	}

	/* *********************************************
	* 工具函数
	* *********************************************/
	static void _correctPerpectUV(float z_start, float z_end, float z_cur,
		const UVPos& uv_start, const UVPos& uv_end, UVPos& uv_cur, float defaultRat) {
		if (JMath::f_equal(1 / z_start, 1 / z_end)) {
			uv_cur = uv_end + (uv_start - uv_end) * defaultRat;
		}
		else {
			float rat = (1 / z_cur - 1 / z_end) / (1 / z_start - 1 / z_end);
			uv_cur = uv_end + (uv_start - uv_end) * rat;
		}
	}

	static float _calRat(float start, float end, float cur) {
		return JMath::f_equal(start, end) ? 0 : ((cur - end) / (start - end));
	}
};

#endif