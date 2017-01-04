#include "SimpleBrush.h"
#include <cmath>
#include <algorithm>

/* *********************************************
* �������ص�
* *********************************************/
void SimpleBrush::DrawDot_normedPos(float x0, float y0, Color4 color, ImgBuffer<Color4>* buffer) {
	*static_cast<Color4*>(buffer->pPixelAt_normedPos(x0, y0)) = color;
}

void SimpleBrush::DrawDot_normedPos(ScreenPos v, Color4 color, ImgBuffer<Color4>* buffer) {
	DrawDot_normedPos(v._x, v._y, color, buffer);
}

void SimpleBrush::DrawDot_coordPos(ScreenCoord x, ScreenCoord y, Color4 color, ImgBuffer<Color4>* buffer){
	*static_cast<Color4*>(buffer->pPixelAt(x, y)) = color;
}

/* *********************************************
* ��ֱ��
* *********************************************/
void SimpleBrush::DrawLine_floatPos(float x0, float y0, float x1, float y1, Color4 color, ImgBuffer<Color4>* buffer) {
	_DrawLine_floatPos_bresenham(x0, y0, x1, y1, color, buffer);
}

void SimpleBrush::DrawLine_floatPos(ScreenPos v1, ScreenPos v2, Color4 color, ImgBuffer<Color4>* buffer) {
	DrawLine_floatPos(v1._x, v1._y, v2._x, v2._y, color, buffer);
}

void SimpleBrush::_DrawLine_floatPos_bresenham(float x0, float y0, float x1, float y1, Color4 color, ImgBuffer<Color4>* buffer) {
	// ��������
	int x_step = (x1 > x0) ? 1 : -1;
	int y_step = (y1 > y0) ? 1 : -1;
	// ���������ص�
	ScreenCoord x_bgn, y_bgn;
	ScreenCoord* p_x_bgn = &x_bgn;
	ScreenCoord* p_y_bgn = &y_bgn;
	// ת��ʹ k<1
	if (std::fabs(y0 - y1) > std::fabs(x0 - x1)){
		std::swap(x_step, y_step);
		std::swap(p_x_bgn, p_y_bgn);
		std::swap(x0, y0);
		std::swap(x1, y1);
	}

	float k = (y0 - y1) / (x0 - x1);
	*p_x_bgn = static_cast<ScreenCoord>(x0);// -(float)x_step / 2);
	ScreenCoord x_end = static_cast<ScreenCoord>(x1);// +(float)x_step / 2);
	*p_y_bgn = static_cast<ScreenCoord>(k*((*p_x_bgn) - x0) + y0);// +0.5);

	float e = (*p_y_bgn) - (k*((*p_x_bgn) - x0) + y0);
	while ((*p_x_bgn) != x_end){
		(*p_x_bgn) += x_step;
		e += k*x_step;
		if (e * y_step > 0.5 * y_step){ // todo �����Ż�
			e -= y_step;
			(*p_y_bgn) += y_step;
		}
		DrawDot_coordPos(x_bgn, y_bgn, color, buffer);
	}
}

void SimpleBrush::_DrawLine_coordPos_h(ScreenCoord x0, ScreenCoord x1, ScreenCoord y, Color4 color, ImgBuffer<Color4>* buffer){
	if (x0 > x1){
		std::swap(x0, x1);
	}

	do{
		DrawDot_coordPos(x0, y, color, buffer);
	} while (++x0 <= x1);
}

void SimpleBrush::_DrawLine_coordPos_h(float x0, float x1, ScreenCoord y, Color4 color, ImgBuffer<Color4>* buffer){
	_DrawLine_coordPos_h(static_cast<ScreenCoord>(x0 + 0.5), static_cast<ScreenCoord>(x1 + 0.5), y, color, buffer);
}

/* *********************************************
* ���������
* *********************************************/
void SimpleBrush::DrawTriangle(ScreenPos v1, ScreenPos v2, ScreenPos v3, Color4 color, ImgBuffer<Color4>* buffer) {
	DrawTriangle(v1._x, v1._y, v2._x, v2._y, v3._x, v3._y, color, buffer);
}

// todo ������ȷ�������Ż�
void SimpleBrush::DrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, Color4 color, ImgBuffer<Color4>* buffer){
	// ת��ʹ������Ϊyֵ��С��������
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
	// ��������
	ScreenCoord y0_i = static_cast<ScreenCoord>(y0);
	ScreenCoord y1_i = static_cast<ScreenCoord>(y1);
	ScreenCoord y2_i = static_cast<ScreenCoord>(y2);
	// �ҵ��ָ���м��
	if (y0_i == y2_i){
		return;
	}
	float x_mid = (y1 - y2) / (y0 - y2) * (x0 - x2) + x2;

	float x_bgn = x0;
	float k02 = (x2 - x0) / (y2 - y0);
	float x_end = x0;
	float k01 = (x1 - x0) / (y1 - y0);
	float k12 = (x1 - x2) / (y1 - y2);
	while (y0_i <= y2_i){
		if (y0_i == y1_i){
			k01 = k12;
			x_end = x1;
			_DrawLine_coordPos_h(x_mid, x1, y1_i, color, buffer);
		}
		else{
			x_bgn += k02;
			x_end += k01;
			_DrawLine_coordPos_h(x_bgn, x_end, y0_i, color, buffer);
		}
		++y0_i;
	}
}