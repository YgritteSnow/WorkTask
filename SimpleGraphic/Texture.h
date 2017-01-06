#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <map>
#include <string>

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
	ColorType GetPixel_normedPos(float x, float y){ return *(pPixelAt_normedPos(x, y)); }

	void clear() { memset(m_data, 0x33, sizeof(ColorType)* width * height); };
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

class TextureManager {
public:
	~TextureManager();

	static bool Init();
	static void UnInit();
	static TextureManager* GetInstance(){ return m_instance; }

public:
	ImgBuffer<ShortColor4>* LoadTexture_short(std::string texname);
	ImgBuffer<ShortColor4>* GetTexture_short(std::string texname);
	ImgBuffer<NormColor4>* LoadTexture_norm(std::string texname);
	ImgBuffer<NormColor4>* GetTexture_norm(std::string texname);
	ImgBuffer<NormColor4>* ConvertShortToNrom(ImgBuffer<ShortColor4>* pTex_short);

private:
	static TextureManager* m_instance;
	std::map<std::string, ImgBuffer<ShortColor4>*> m_map_tex_short;
	std::map<std::string, ImgBuffer<NormColor4>*> m_map_tex_norm;
};

#endif
