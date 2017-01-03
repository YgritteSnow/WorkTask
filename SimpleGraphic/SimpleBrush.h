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
	void clear() { memset(m_data, 0x0, sizeof(ColorType) * m_width * m_height); };
	ColorType* pPixelAt(ScreenCoord x, ScreenCoord y) const { return m_data + y * m_width + x; };
	ColorType* pPixelAt_normedPos(float x, float y) const { return m_data + (int)(y*m_height+0.5) * m_width + (int)(x*m_width+0.5); };
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
	static void DrawDot_screen(float x0, float y0, Color4 color, ImgBuffer<Color4>* buffer) {
		*(buffer->pPixelAt_normedPos(x0, y0)) = color;
	}
	static void DrawDot_screen(ScreenPos v, Color4 color, ImgBuffer<Color4>* buffer);
	// »­Ö±Ïß
	static void DrawLine_screen(float x0, float y0, float x1, float y1, ImgBuffer<Color4>* buffer);
	static void DrawLine_screen(ScreenPos v1, ScreenPos v2, ImgBuffer<Color4>* buffer);
	static void _DrawLine_screen_bresenham(float x0, float y0, float x1, float y1, ImgBuffer<Color4>* buffer);
};

#endif