#ifndef __SIMPLE_BRUSH_H__
#define __SIMPLE_BRUSH_H__

#include <memory.h>
#include "JMath.h"
#include "Color.h"
#include "Texture.h"
#include "TargetBuffer.h"

// ֱ�Ӵ������ݽ��л�ͼ
class SimpleBrush{
public:
	// �������ص�
	static void DrawDot_normedPos(
		float x0, float y0, NormColor4 color,
		ImgBuffer<ShortColor4>* back_buffer);
	static void DrawDot_normedPos(
		ScreenPos v, NormColor4 color,
		ImgBuffer<ShortColor4>* back_buffer);
	static void DrawDot_coordPos(
		ScreenCoord x, ScreenCoord y, NormColor4 color,
		ImgBuffer<ShortColor4>* back_buffer);

public:
	// ��ֱ��
	static void DrawLine_floatPos(
		float x0, float y0, NormColor4 color0,
		float x1, float y1, NormColor4 color1,
		ImgBuffer<ShortColor4>* back_buffer);
	static void DrawLine_floatPos(
		ScreenPos v1, NormColor4 color0,
		ScreenPos v2, NormColor4 color1,
		ImgBuffer<ShortColor4>* back_buffer);
public:
	static void _DrawLine_floatPos_bresenham(
		float x0, float y0, NormColor4 color0,
		float x1, float y1, NormColor4 color1,
		ImgBuffer<ShortColor4>* back_buffer);
	static void _DrawLine_coordPos_h(
		ScreenCoord x0, NormColor4 color0,
		ScreenCoord x1, NormColor4 color1,
		ScreenCoord y,
		ImgBuffer<ShortColor4>* back_buffer);
	static void _DrawLine_coordPos_h(
		float x0, NormColor4 color0,
		float x1, NormColor4 color1,
		ScreenCoord y,
		ImgBuffer<ShortColor4>* back_buffer);
public:
	// ���������
	static void DrawTriangle(
		ScreenPos v1, NormColor4 color1,
		ScreenPos v2, NormColor4 color2,
		ScreenPos v3, NormColor4 color3,
		ImgBuffer<ShortColor4>* back_buffer);
	static void DrawTriangle(
		float x0, float y0, NormColor4 color0,
		float x1, float y1, NormColor4 color1,
		float x2, float y2, NormColor4 color2,
		ImgBuffer<ShortColor4>* back_buffer);
};

#endif