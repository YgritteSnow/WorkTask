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

// ����Ĳ����Ժ�Ӧ�������ṩ��Դ��handle
void Mesh::Load() { OnLoad(); }

VertexBuffer* Mesh::GetVertexBuffer() { return m_vertexBuffer; }

IndexBuffer* Mesh::GetIndexBuffer() { return m_indexBuffer; }