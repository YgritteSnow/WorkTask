#include "Mesh.h"

Mesh::Mesh()
	: m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{}

Mesh::~Mesh() {
	delete m_vertexBuffer;
	m_vertexBuffer = nullptr;
	delete m_indexBuffer;
	m_indexBuffer = nullptr;
}

// 这里的参数以后应该至少提供资源的handle
void Mesh::Load() { OnLoad(); }

VertexBuffer* Mesh::GetVertexBuffer() { return m_vertexBuffer; }

IndexBuffer* Mesh::GetIndexBuffer() { return m_indexBuffer; }