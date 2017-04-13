#pragma once
#include "StructMeta.h"
#include "SkinnedMesh.h"
#include "VertexUtilities.h"

class TestSnakeMesh : public SkinnedMesh
{
private:
	virtual void OnLoad() override {
		Dummy();
		ProcessVertex_calNormal(
			static_cast<TestSkinnedVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestSkinnedVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
		ProcessVertex_calBiNormal(
			static_cast<TestSkinnedVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestSkinnedVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
	}
	void Dummy() {
		m_vertexBuffer = new VertexBuffer(GetID(TestSkinnedVertex), 4);
		m_indexBuffer = new IndexBuffer(12);

		m_vertexBuffer->SetVertex(0, TestSkinnedVertex(-1, -1, -1, 1, 0, 0, 1));
		m_vertexBuffer->SetVertex(1, TestSkinnedVertex(-1,  1, -1, 1, 1, 0, 1));
		m_vertexBuffer->SetVertex(2, TestSkinnedVertex(-1,  1,  1, 0, 0, 1, 1));
		m_vertexBuffer->SetVertex(3, TestSkinnedVertex( 1,  1,  1, 0, 1, 1, 1));

		m_indexBuffer->m_vec_indice[0] = 0;
		m_indexBuffer->m_vec_indice[1] = 1;
		m_indexBuffer->m_vec_indice[2] = 2;
		m_indexBuffer->m_vec_indice[3] = 2;
		m_indexBuffer->m_vec_indice[4] = 1;
		m_indexBuffer->m_vec_indice[5] = 3;
		m_indexBuffer->m_vec_indice[6] = 3;
		m_indexBuffer->m_vec_indice[7] = 1;
		m_indexBuffer->m_vec_indice[8] = 0;
		m_indexBuffer->m_vec_indice[9] = 3;
		m_indexBuffer->m_vec_indice[10] = 0;
		m_indexBuffer->m_vec_indice[11] = 2;
	}
};