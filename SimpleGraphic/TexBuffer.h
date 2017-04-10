#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <map>
#include <string>

#include "JMath.h"
#include "Color.h"
#include "StructReflection.h"

typedef float DepthBufferPixel;

// Í¼Æ¬»º´æ
template< typename _ColorType >
class TexBuffer {
public:
	typedef _ColorType PixelType;
	TexBuffer(ScreenCoord width, ScreenCoord height)
		:width(width)
		, height(height)
		, m_data(nullptr)
	{
		m_data = new PixelType[width*height];
	};
	~TexBuffer() {
		delete m_data;
		m_data = nullptr;
	}
public:
	PixelType GetPixel_normedPos(float x, float y){ return *(pPixelAt_normedPos(x, y)); }
	PixelType GetPixel_coordPos(ScreenCoord x, ScreenCoord y) { return *(pPixelAt(x, y)); }
	PixelType GetPixel_normedPos_smart(float x, float y) { return *(pPixelAt_normedPos(max(0,min(1,x)), max(0, min(1, y)))); }
	PixelType GetPixel_coordPos_smart(ScreenCoord x, ScreenCoord y) { return *(pPixelAt(max(0, min(1, x)), max(0, min(1, y)))); }

	void clear() { memset(m_data, 0, sizeof(PixelType)* width * height); };
	void clear(PixelType c) { for (int i = 0; i < width * height; ++i) { m_data[i] = c; }; }
	PixelType* pPixelAt(ScreenCoord x, ScreenCoord y) const { return m_data + y * width + x; };
	PixelType* pPixelAt_normedPos(float x, float y) const { return m_data + (int)(y*(height - 1) + 0.5) * width + (int)(x*(width - 1) + 0.5); };
	PixelType* pLineAt(ScreenCoord y) const { return m_data + y * width; };
	PixelType* pRowAt(ScreenCoord x) const { return m_data + x * height; };
	PixelType* pHead() const { return m_data; };
	unsigned PixelSize() { return sizeof(PixelType); }
	const ScreenCoord& GetWidth(){ return width; }
	const ScreenCoord& GetHeight(){ return height; }

	void SetPixelAt(ScreenCoord x, ScreenCoord y, const PixelType& c) {
		*(pPixelAt(x, y)) = c;
	}
	void SetPixelAt_normedPos(float x, float y, const PixelType& c) {
		&(pPixelAt_normedPos(x, y)) = c;
	}
	void SetPixelAt_byVertex(ScreenCoord x, ScreenCoord y, const StructWrapper& s) {
		*(pPixelAt(x, y)) = s;
	}
	void SetPixelAt_byVertex_normedPos(float x, float y, const StructWrapper& s) {
		*(pPixelAt_normedPos(x, y)) = s;
	}

public:
	ScreenCoord width, height;
	PixelType* m_data;
};

#endif
