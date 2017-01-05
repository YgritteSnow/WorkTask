#ifndef __SIMPLE_BRUSH_H__
#define __SIMPLE_BRUSH_H__

#include <memory.h>
#include "JMath.h"
#include "Color.h"

// Í¼Æ¬»º´æ
template< typename ColorType >
class ImgBuffer {
public:
	ImgBuffer(ScreenCoord width, ScreenCoord height)
		:width(width)
		, height(height)
		, m_data(nullptr)
	{
		m_data = new ColorType[width*height];
	};
	~ImgBuffer() {
		delete m_data;
		m_data = nullptr;
	}
public:
	void clear() { memset(m_data, 0x33, sizeof(ColorType) * width * height); };
	ColorType* pPixelAt(ScreenCoord x, ScreenCoord y) const { return m_data + y * width + x; };
	ColorType* pPixelAt_normedPos(float x, float y) const { return m_data + (int)(y*(height - 1) + 0.5) * width + (int)(x*(width - 1) + 0.5); };
	ColorType* pLineAt(ScreenCoord y) const { return m_data + y * width; };
	ColorType* pRowAt(ScreenCoord x) const { return m_data + x * height; };
	ColorType* pHead() const { return m_data; };
	unsigned PixelSize() { return sizeof(ColorType); }
	ScreenCoord GetWidth(){ return width; }
	ScreenCoord GetHeight(){ return height; }

public:
	ScreenCoord width, height;
	ColorType* m_data;
};

// Ò»Ð©»­Í¼µÄ±ÊË¢
class SimpleBrush{
public:
	// »­µ¥ÏñËØµã
	static void DrawDot_normedPos(
		float x0, float y0, NormColor4 color,
		ImgBuffer<ShortColor4>* buffer);
	static void DrawDot_normedPos(
		ScreenPos v, NormColor4 color,
		ImgBuffer<ShortColor4>* buffer);
	static void DrawDot_coordPos(
		ScreenCoord x, ScreenCoord y, NormColor4 color,
		ImgBuffer<ShortColor4>* buffer);

	// »­Ö±Ïß
public:
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

	// Ìî³äÈý½ÇÐÎ
public:
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