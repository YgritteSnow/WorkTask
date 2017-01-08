#ifndef __TARGETBUFFER_H__
#define __TARGETBUFFER_H__

#include <vector>
#include <algorithm>

#include "Color.h"
#include "Texture.h"

/* *********************************************
* alpha buffer
* *********************************************/
struct AlphaBlendData {
	AlphaBlendData(NormColor4 c, DepthBufferPixel d)
		: color(c), depth(d) {}

	NormColor4 color;
	DepthBufferPixel depth;
};
struct AlphaBlendCpr {
	bool operator()(const AlphaBlendData& a1, const AlphaBlendData& a2) {
		return a1.depth > a2.depth;
	}
};

struct AlphaBlendFrag {
	AlphaBlendFrag() :m_vec_color(), m_blended_color() {}
	void clear() {
		m_vec_color.clear();
		m_blended_color.Zero();
	}
	void SortAndBlend() {
		std::sort(m_vec_color.begin(), m_vec_color.end(), AlphaBlendCpr());
		m_blended_color = NormColor4(0, 0, 0, 0);
		float w = 0;
		for (auto it = m_vec_color.begin(); it != m_vec_color.end(); ++it) {
			w = 1 - (1 - w) * (1 - it->color._w);
			m_blended_color = BlendColor(m_blended_color, it->color);
			m_blended_color._w = w;
		}
	}

	std::vector<AlphaBlendData> m_vec_color;
	NormColor4 GetNormColor4() { return NormColor4(); }
	NormColor4 m_blended_color;
};

class AlphaBuffer : public ImgBuffer<AlphaBlendFrag>{
public:
	AlphaBuffer(ScreenCoord width, ScreenCoord height) :ImgBuffer<AlphaBlendFrag>(width, height) {}

	void clear() {
		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				pPixelAt(x, y)->clear();
			}
		}
	}

	template <typename VertexStruct>
	void SetPixelAt_byVertex(ScreenCoord x, ScreenCoord y, const VertexStruct& v) {
		pPixelAt(x, y)->m_vec_color.push_back(AlphaBlendData(v.color, v.pos._z));
	}
	template <typename VertexStruct>
	void SetPixelAt_byVertex_normedPos(float x, float y, const VertexStruct& v) {
		pPixelAt_normedPos(x, y)->m_vec_color.push_back(AlphaBlendData(v.color, v.pos._z));
	}

	void SortAndBlend() {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				pPixelAt(x, y)->SortAndBlend();
			}
		}
	}
};
#endif