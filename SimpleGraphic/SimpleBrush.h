#ifndef __SIMPLE_BRUSH_H__
#define __SIMPLE_BRUSH_H__

#include <memory.h>
#include "JMath.h"

// Í¼Æ¬»º´æ
template< typename ColorType >
class ImgBuffer {
public:
	ImgBuffer(ScreenCoord width, ScreenCoord height)
		:m_width(width)
		, m_height(height)
		, m_data(new ColorType[width*height])
	{};
public:
	void clear() { memset(m_data, 0x33, sizeof(ColorType) * m_width * m_height); };
	ColorType* pPixelAt(ScreenCoord x, ScreenCoord y) const { return m_data + y * m_width + x; };
	ColorType* pPixelAt_normedPos(float x, float y) const { 
		auto tmp = (int)(y*(m_height-1) + 0.5) * m_width + (int)(x*(m_width-1) + 0.5);
		return m_data + tmp;
		//return m_data + (int)(y*m_height+0.5) * m_width + (int)(x*m_width+0.5); 
	};
	ColorType* pLineAt(ScreenCoord y) const { return m_data + y * m_width; };
	ColorType* pRowAt(ScreenCoord x) const { return m_data + x * m_height; };
	ColorType* pHead() const { return m_data; };
	unsigned PixelSize() { return sizeof(ColorType); }
private:
	ScreenCoord m_width, m_height;
	ColorType* m_data;
};

// Ò»Ð©»­Í¼µÄ±ÊË¢
class SimpleBrush{
public:
	// »­µ¥ÏñËØµã
	static void DrawDot_normedPos(
		float x0, float y0, 
		Color4 color, ImgBuffer<Color4>* buffer);
	static void DrawDot_normedPos(
		ScreenPos v, 
		Color4 color, ImgBuffer<Color4>* buffer);
	static void DrawDot_coordPos(
		ScreenCoord x, ScreenCoord y, 
		Color4 color, ImgBuffer<Color4>* buffer);

	// »­Ö±Ïß
	static void DrawLine_floatPos(
		float x0, float y0, 
		float x1, float y1, 
		Color4 color, ImgBuffer<Color4>* buffer);
	static void DrawLine_floatPos(
		ScreenPos v1, 
		ScreenPos v2, 
		Color4 color, ImgBuffer<Color4>* buffer);
	static void _DrawLine_floatPos_bresenham(
		float x0, float y0, 
		float x1, float y1, 
		Color4 color, ImgBuffer<Color4>* buffer);
	static void _DrawLine_coordPos_h(
		ScreenCoord x0, ScreenCoord x1,
		ScreenCoord y,
		Color4 color, ImgBuffer<Color4>* buffer);
	static void _DrawLine_coordPos_h(
		float x0, float x1,
		ScreenCoord y,
		Color4 color, ImgBuffer<Color4>* buffer);

	// Ìî³äÈý½ÇÐÎ
	static void DrawTriangle(
		ScreenPos v1, 
		ScreenPos v2, 
		ScreenPos v3, 
		Color4 color, ImgBuffer<Color4>* buffer);
	static void DrawTriangle(
		float x0, float y0,
		float x1, float y1,
		float x2, float y2,
		Color4 color, ImgBuffer<Color4>* buffer);
};

#endif