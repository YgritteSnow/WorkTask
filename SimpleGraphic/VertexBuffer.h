#pragma once
#include <Windows.h>
#include <vector>

#include "StructReflection.h"

class VertexBuffer
{
public:
	VertexBuffer(STRUCT_ID vertexid, int length) { _innerInit(vertexid, length); }
	~VertexBuffer() { free(m_vec_vertex); m_vec_vertex = nullptr; };
public:
	auto size() { return m_length; }

	template <typename T>
	inline void SetVertex(int index, T&& v)
	{
		memcpy(m_vec_vertex + index*m_size, &v, m_size);
	}
public:
	inline byte* GetVertex(int index) const { return m_vec_vertex + m_size * index; }
	inline STRUCT_ID GetId() const { return m_vid; }
//private:
	byte* m_vec_vertex;
	int m_length;
	STRUCT_ID m_vid;
	int m_size;
private:
	void _innerInit(STRUCT_ID vertexid, int length) {
		m_vid = vertexid;
		auto vertex_size = StructReflectManager::_id2ref[vertexid].m_size;
		m_vec_vertex = static_cast<byte*>(malloc(length * vertex_size));
		m_length = length;
		m_size = vertex_size;
	}
};

class IndexBuffer
{
public:
	IndexBuffer(int length) { m_vec_indice = new DWORD[length](); m_length = length; };
	~IndexBuffer() { delete[] m_vec_indice; m_vec_indice = nullptr; m_length = 0; };
	const DWORD& operator[](int index) const{ return *(m_vec_indice + index); }
public:
	auto size() const { return m_length; }
//private:
	DWORD* m_vec_indice;
	int m_length;
};