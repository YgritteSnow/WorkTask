#include "SimpleBrush.h"
#include <cmath>
#include <algorithm>

/* *********************************************
* 画单像素点
* *********************************************/
void SimpleBrush::DrawDot_normedPos(float x, float y, NormColor4 color, ImgBuffer<ShortColor4>* buffer) {
	if (x >= 0 && y >= 0 && x < 1 && y < 1){
		*static_cast<ShortColor4*>(buffer->pPixelAt_normedPos(x, y)) = static_cast<ShortColor4>(color);
	}
}

void SimpleBrush::DrawDot_normedPos(ScreenPos v, NormColor4 color, ImgBuffer<ShortColor4>* buffer) {
	DrawDot_normedPos(v._x, v._y, color, buffer);
}

void SimpleBrush::DrawDot_coordPos(ScreenCoord x, ScreenCoord y, NormColor4 color, ImgBuffer<ShortColor4>* buffer){
	if (x >= 0 && y >= 0 && x < buffer->width && y < buffer->height){
		*static_cast<ShortColor4*>(buffer->pPixelAt(x, y)) = static_cast<ShortColor4>(color);
	}
}

/* *********************************************
* 画直线
* *********************************************/
void SimpleBrush::DrawLine_floatPos(float x0, float y0, NormColor4 color0, float x1, float y1, NormColor4 color1, ImgBuffer<ShortColor4>* buffer) {
	_DrawLine_floatPos_bresenham(
		x0, y0, static_cast<LongColor4>(color0), 
		x1, y1, static_cast<LongColor4>(color1),
		buffer);
}

void SimpleBrush::DrawLine_floatPos(ScreenPos v1, NormColor4 color0, ScreenPos v2, NormColor4 color1, ImgBuffer<ShortColor4>* buffer) {
	DrawLine_floatPos(v1._x, v1._y, color0, v2._x, v2._y, color1, buffer);
}

void SimpleBrush::_DrawLine_floatPos_bresenham(
	float x0, float y0, LongColor4 color0,
	float x1, float y1, LongColor4 color1,
	ImgBuffer<ShortColor4>* buffer) {
	// 步进长度
	int x_step = (x1 > x0) ? 1 : -1;
	int y_step = (y1 > y0) ? 1 : -1;
	// 步进的像素点
	ScreenCoord x_bgn, y_bgn;
	ScreenCoord* p_x_bgn = &x_bgn;
	ScreenCoord* p_y_bgn = &y_bgn;
	// 转换使 k<1
	if (std::fabs(y0 - y1) > std::fabs(x0 - x1)){
		std::swap(x_step, y_step);
		std::swap(p_x_bgn, p_y_bgn);
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	// 颜色插值
	LongColor4 color_bgn = color0;
	LongColor4 color_step = (color1 - color0) * ((float)x_step / (x1 - x0));

	float k = (y0 - y1) / (x0 - x1);
	*p_x_bgn = static_cast<ScreenCoord>(x0);// -(float)x_step / 2);
	ScreenCoord x_end = static_cast<ScreenCoord>(x1);// +(float)x_step / 2);
	*p_y_bgn = static_cast<ScreenCoord>(k*((*p_x_bgn) - x0) + y0);// +0.5);

	float e = (*p_y_bgn) - (k*((*p_x_bgn) - x0) + y0);
	while ((*p_x_bgn) != x_end){
		(*p_x_bgn) += x_step;
		color_bgn += color_step;
		e += k*x_step;
		if (e * y_step > 0.5 * y_step){ // todo 可以优化
			e -= y_step;
			(*p_y_bgn) += y_step;
		}
		DrawDot_coordPos(x_bgn, y_bgn, color_bgn, buffer);
	}
}

void SimpleBrush::_DrawLine_coordPos_h(
	ScreenCoord x0, LongColor4 color0,
	ScreenCoord x1, LongColor4 color1,
	ScreenCoord y,
	ImgBuffer<ShortColor4>* buffer){
	if (x0 > x1){
		std::swap(x0, x1);
	}

	LongColor4 color_bgn = color0;
	LongColor4 color_step = (color1 - color0) * (1.f / (x1 - x0));
	do{
		DrawDot_coordPos(x0, y, color_bgn, buffer);
		color_bgn += color_step;
	} while (++x0 <= x1);
}

void SimpleBrush::_DrawLine_coordPos_h(
	float x0, LongColor4 color0,
	float x1, LongColor4 color1,
	ScreenCoord y,
	ImgBuffer<ShortColor4>* buffer) {
	_DrawLine_coordPos_h(static_cast<ScreenCoord>(x0 + 0.5), color0, static_cast<ScreenCoord>(x1 + 0.5), color1, y, buffer);
}

/* *********************************************
* 填充三角形
* *********************************************/
void SimpleBrush::DrawTriangle(
	ScreenPos v1, NormColor4 color1,
	ScreenPos v2, NormColor4 color2,
	ScreenPos v3, NormColor4 color3,
	ImgBuffer<ShortColor4>* buffer) {
	DrawTriangle(v1._x, v1._y, color1,
		v2._x, v2._y, color2,
		v3._x, v3._y, color3, buffer);
}

// todo 不够精确，可以优化
void SimpleBrush::DrawTriangle(
	float x0, float y0, NormColor4 color0,
	float x1, float y1, NormColor4 color1,
	float x2, float y2, NormColor4 color2,
	ImgBuffer<ShortColor4>* buffer) {
	// 转换使三个点为y值从小到大排列
	if (y0 > y1){
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	if (y0 > y2){
		std::swap(x0, x2);
		std::swap(y0, y2);
	}
	if (y1 > y2){
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	// 整数格子
	ScreenCoord y0_i = static_cast<ScreenCoord>(y0);
	ScreenCoord y1_i = static_cast<ScreenCoord>(y1);
	ScreenCoord y2_i = static_cast<ScreenCoord>(y2);
	// 找到分割的中间点
	if (y0_i == y2_i){
		return;
	}

	float x_left = x0;
	float k02 = (x2 - x0) / (y2 - y0);
	float x_right = x0;
	float k01 = (x1 - x0) / (y1 - y0);
	float k12 = (x1 - x2) / (y1 - y2);

	LongColor4 color0_long = static_cast<LongColor4>(color0);
	LongColor4 color1_long = static_cast<LongColor4>(color1);
	LongColor4 color2_long = static_cast<LongColor4>(color2);
	LongColor4 color_left = color0_long;
	LongColor4 color02 = (1.f / (y2 - y0))*(color2_long - color0_long);
	LongColor4 color_right = color0_long;
	LongColor4 color01 = (1.f / (y1 - y0)) * (color1_long - color0_long);
	LongColor4 color12 = (1.f / (y2 - y1))*(color2_long - color1_long);

	while (y0_i <= y2_i){
		x_left += k02;
		color_left += color02;
		if (y0_i == y1_i){
			k01 = k12;
			x_right = x1;
			color01 = color12;
			color_right = color1_long;
			_DrawLine_coordPos_h(x_left, color_left, x1, color_right, y1_i, buffer);
		}
		else{
			x_right += k01;
			color_right += color01;
			_DrawLine_coordPos_h(x_left, color_left, x_right, color_right, y0_i, buffer);
		}
		++y0_i;
	}
}