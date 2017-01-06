#ifndef __SIMPLE_BRUSH_H__
#define __SIMPLE_BRUSH_H__

#include <memory.h>
#include "JMath.h"
#include "Color.h"
#include "Texture.h"

// 一些画图的笔刷
class SimpleBrush{
public:
	// 对两条线上的某个插值点，根据深度计算其 uv
	static float _getLerpRatio_noCorrect(WorldPos v0, WorldPos v1, float rat){
		return rat;
	}
	static UVPos _getLerpRatio_withCorrect(WorldPos v0, WorldPos v1, float rat){
		float v0_xz = v0._x / v0._z;
		float v0_yz = v0._y / v0._z;
		float v1_xz = v1._x / v1._z;
		float v1_yz = v1._y / v1._z;

		float x_ratuo = (((rat * (v0_xz - v1_xz)) + v1_xz) - v0._x) / (v1._x - v0._x);
		float y_ratuo = (((rat * (v0_yz - v1_yz)) + v1_yz) - v0._y) / (v1._y - v0._y);
		return UVPos(x_ratuo, y_ratuo);
	}

public:
	// 画单像素点
	static void DrawDot_normedPos(
		float x0, float y0, NormColor4 color,
		ImgBuffer<ShortColor4>* buffer);
	static void DrawDot_normedPos(
		ScreenPos v, NormColor4 color,
		ImgBuffer<ShortColor4>* buffer);
	static void DrawDot_coordPos(
		ScreenCoord x, ScreenCoord y, NormColor4 color,
		ImgBuffer<ShortColor4>* buffer);

public:
	// 画直线
	static void DrawLine_floatPos(
		float x0, float y0, NormColor4 color0,
		float x1, float y1, NormColor4 color1,
		ImgBuffer<ShortColor4>* buffer);
	static void DrawLine_floatPos(
		ScreenPos v1, NormColor4 color0,
		ScreenPos v2, NormColor4 color1,
		ImgBuffer<ShortColor4>* buffer);
private:
	static void _DrawLine_floatPos_bresenham(
		float x0, float y0, LongColor4 color0,
		float x1, float y1, LongColor4 color1,
		ImgBuffer<ShortColor4>* buffer);
	static void _DrawLine_coordPos_h(
		ScreenCoord x0, LongColor4 color0,
		ScreenCoord x1, LongColor4 color1,
		ScreenCoord y,
		ImgBuffer<ShortColor4>* buffer);
	static void _DrawLine_coordPos_h(
		float x0, LongColor4 color0,
		float x1, LongColor4 color1,
		ScreenCoord y,
		ImgBuffer<ShortColor4>* buffer);
public:
	// 填充三角形
	static void DrawTriangle(
		ScreenPos v1, NormColor4 color1,
		ScreenPos v2, NormColor4 color2,
		ScreenPos v3, NormColor4 color3,
		ImgBuffer<ShortColor4>* buffer);
	static void DrawTriangle(
		float x0, float y0, NormColor4 color0,
		float x1, float y1, NormColor4 color1,
		float x2, float y2, NormColor4 color2,
		ImgBuffer<ShortColor4>* buffer);
};

#endif