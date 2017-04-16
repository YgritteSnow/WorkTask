#include "SkinnedMesh.h"

#include <minwindef.h>
#include "StructReflection.h"

SkinnedMesh::SkinnedMesh()
	: m_animator(nullptr)
{}

SkinnedMesh::~SkinnedMesh() {
	delete m_animator;
	m_animator = nullptr;
	delete[] m_vec_vpos;
	m_vec_vpos = nullptr;
}

void SkinnedMesh::Load() {
	OnLoad();
	m_vec_vpos = new WorldPos[m_vertexBuffer->m_length];
	STRUCT_ID offset = StructReflectManager::GetOffset<POSITION>(m_vertexBuffer->GetId(), 0);
	for (int i = 0; i < m_vertexBuffer->m_length; ++i) {
		m_vec_vpos[i] = *static_cast<WorldPos*>(static_cast<void*>(m_vertexBuffer->GetVertex(i) + offset));
	}
}

void SkinnedMesh::BeforeRender() {
	if (m_animator->IsDirty()) {
		RefreshBySkeleton();
		m_animator->Cleaned();
	}
}

void SkinnedMesh::RefreshBySkeleton() {
	STRUCT_ID pos_offset = StructReflectManager::GetOffset<POSITION>(m_vertexBuffer->GetId(), 0);
	STRUCT_ID vb_offset = StructReflectManager::GetOffset<VERTEXBONE>(m_vertexBuffer->GetId(), 0);
	// 遍历所有顶点
	for (int iv = 0; iv < m_vertexBuffer->size(); ++iv) {
		byte* v = m_vertexBuffer->GetVertex(iv);
		WorldPos* pos = static_cast<WorldPos*>(static_cast<void*>(v + pos_offset));
		VertexBone* vb = static_cast<VertexBone*>(static_cast<void*>(v + vb_offset));
		// 遍历顶点所绑定到的所有骨骼点
		WorldPos new_pos;
		for (int ib = 0; ib < vb->bone_count; ++ib) {
			auto bone_index = vb->bone_index[ib];
			// 计算该骨骼点下顶点的实际位置，按照权重进行叠加
			new_pos += m_animator->CalculateVertexPos(bone_index, m_vec_vpos[iv]) * vb->bone_weight[ib];
		}
		*pos = new_pos;
	}
}