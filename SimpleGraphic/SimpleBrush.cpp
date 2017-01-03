#include "SimpleBrush.h"

// 画单像素点
//void SimpleBrush::DrawDot_screen(float x0, float y0, Color4 color, ImgBuffer<Color4>* buffer) {
//	*(buffer->pPixelAt_normedPos(x0, y0)) = color;
//}
void SimpleBrush::DrawDot_screen(ScreenPos v, Color4 color, ImgBuffer<Color4>* buffer) {
	DrawDot_screen(v._x, v._y, color, buffer);
}

// 画直线
void SimpleBrush::DrawLine_screen(float x0, float y0, float x1, float y1, ImgBuffer<Color4>* buffer) {
	_DrawLine_screen_bresenham(x0, y0, x1, y1, buffer);
}

void SimpleBrush::DrawLine_screen(ScreenPos v1, ScreenPos v2, ImgBuffer<Color4>* buffer) {
	DrawLine_screen(v1._x, v1._y, v2._x, v2._y, buffer);
}

void SimpleBrush::_DrawLine_screen_bresenham(float x0, float y0, float x1, float y1, ImgBuffer<Color4>* buffer) {

}