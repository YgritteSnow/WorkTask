#ifndef __TARGETBUFFER_H__
#define __TARGETBUFFER_H__

#define USE_BUDDLESORT 1
#define SORT_WHILE_ADD USE_BUDDLESORT
#define ALPHA_OP >

#if USE_BUDDLESORT
#include <algorithm>
const int DEFAULT_ALPHABUFFER_SIZE = 2;
#else
#include <vector>
#include <algorithm>
#endif

#include "Color.h"
#include "TexBuffer.h"
#include "StructReflection.h"

/* *********************************************
* alpha buffer
* *********************************************/
struct AlphaBlendData {
#if USE_BUDDLESORT
	AlphaBlendData() :color(), depth(0){}
#endif
	AlphaBlendData(NormColor4 c, DepthBufferPixel d)
		: color(c), depth(d) {}

	NormColor4 color;
	DepthBufferPixel depth;
};

struct AlphaBlendCpr {
	bool operator()(const AlphaBlendData& a1, const AlphaBlendData& a2) {
		return a1.depth ALPHA_OP a2.depth;
	}
};

struct AlphaBlendFrag {
	AlphaBlendFrag()
#if USE_BUDDLESORT
		:m_vec_color()
		, m_size(0)
		, m_capacity(DEFAULT_ALPHABUFFER_SIZE)
#else
		:m_vec_color()
#endif
		, m_blended_color() 
	{
#if USE_BUDDLESORT
		m_vec_color = new AlphaBlendData[DEFAULT_ALPHABUFFER_SIZE];
#endif
	}
	AlphaBlendFrag(NormColor4 c)
#if USE_BUDDLESORT
		:m_vec_color()
		, m_size(1)
		, m_capacity(DEFAULT_ALPHABUFFER_SIZE)
#else
		: m_vec_color()
#endif
		, m_blended_color()
	{
#if USE_BUDDLESORT
		m_vec_color = new AlphaBlendData[DEFAULT_ALPHABUFFER_SIZE];
		m_vec_color[0] = AlphaBlendData(c, 0);
#endif
	}
#if USE_BUDDLESORT
	~AlphaBlendFrag(){
		delete[] m_vec_color;
		m_vec_color = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

private:
	AlphaBlendFrag(const AlphaBlendFrag& f)
		: m_size(f.m_size)
		, m_capacity(f.m_capacity)
		, m_blended_color(f.m_blended_color)
	{
		m_vec_color = new AlphaBlendData[m_capacity];
		memcpy(m_vec_color, f.m_vec_color, sizeof(AlphaBlendData)* m_size);
	}

public:
#endif
	void clear() {
#if USE_BUDDLESORT
		m_size = 0;
#else
		m_vec_color.clear();
#endif
		m_blended_color.Zero();
	}

	void AddColor(const AlphaBlendData& c){
#if USE_BUDDLESORT
		if (m_size >= m_capacity){
			m_capacity <<= 1;
			AlphaBlendData* new_vec = new AlphaBlendData[m_capacity];
			memcpy(new_vec, m_vec_color, sizeof(AlphaBlendData) * m_capacity / 2);
			delete[] m_vec_color;
			m_vec_color = new_vec;
		}
		m_vec_color[m_size] = c;
		++m_size;
#else
		m_vec_color.push_back(c);
#endif

#if SORT_WHILE_ADD
		SortLastAdd();
#endif
	}

	void SortLastAdd(){
#if USE_BUDDLESORT
		for( int idx = m_size-1; idx > 0; --idx){
			if (m_vec_color[idx].depth ALPHA_OP m_vec_color[idx - 1].depth){
				std::swap(m_vec_color[idx], m_vec_color[idx - 1]);
			}
		}
#else
		Sort();
#endif
	}

	void Sort(){
#if USE_BUDDLESORT
		if (m_size <= 1){ return; }
		std::sort(m_vec_color, m_vec_color+m_size, AlphaBlendCpr());
#else
		if (m_vec_color.size() <= 1){ return; }
		std::sort(m_vec_color.begin(), m_vec_color.end(), AlphaBlendCpr());
#endif
	}

	void Blend() {
#if !SORT_WHILE_ADD
		Sort();
#endif

		m_blended_color = NormColor4(0, 0, 0, 0);
		float w = 0;
#if USE_BUDDLESORT
		for (int idx = 0; idx < m_size; ++idx){
			w = 1 - (1 - w) * (1 - (m_vec_color + idx)->color._w);
			m_blended_color = BlendColor(m_blended_color, (m_vec_color+idx)->color);
			m_blended_color._w = w;
		}
#else
		for (auto it = m_vec_color.begin(); it != m_vec_color.end(); ++it) {
			w = 1 - (1 - w) * (1 - it->color._w);
			m_blended_color = BlendColor(m_blended_color, it->color);
			m_blended_color._w = w;
		}
#endif
	}

#if USE_BUDDLESORT
	AlphaBlendData* m_vec_color;
	int m_size;
	int m_capacity;
#else
	std::vector<AlphaBlendData> m_vec_color;
#endif
	NormColor4 m_blended_color;
};

class AlphaBuffer : public TexBuffer<AlphaBlendFrag>{
public:
	AlphaBuffer(ScreenCoord width, ScreenCoord height) :TexBuffer<AlphaBlendFrag>(width, height) {}

	void clear() {
		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				pPixelAt(x, y)->clear();
			}
		}
	}

	void SetPixelAt_byVertex(ScreenCoord x, ScreenCoord y, STRUCT_ID vid, const byte* v) {
		const auto& vpos = *static_cast<const WorldPos*>(static_cast<const void*>(v + StructReflectManager::GetOffset<POSITION>(vid, 0)));
		const auto& vcolor = *static_cast<const NormColor4*>(static_cast<const void*>(v + StructReflectManager::GetOffset<COLOR>(vid, 0)));
		pPixelAt(x, y)->AddColor(AlphaBlendData(vcolor, vpos._z));
	}
	void SetPixelAt_byVertex_normedPos(float x, float y, STRUCT_ID vid, const byte* v) {
		const auto& vpos = *static_cast<const WorldPos*>(static_cast<const void*>(v + StructReflectManager::GetOffset<POSITION>(vid, 0)));
		const auto& vcolor = *static_cast<const NormColor4*>(static_cast<const void*>(v + StructReflectManager::GetOffset<COLOR>(vid, 0)));
		pPixelAt_normedPos(x, y)->AddColor(AlphaBlendData(vcolor, vpos._z));
	}

	void Blend() {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				pPixelAt(x, y)->Blend();
			}
		}
	}
};
#endif