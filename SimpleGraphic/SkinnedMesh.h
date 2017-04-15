#pragma once

#include "Mesh.h"
#include "Skelenton.h"
#include "Animator.h"

class SkinnedMesh : public Mesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();

private:
	// ���ݹ�����������¶��㻺��
	void RefreshVertexBuffer();

private:
	Animator* m_animator;
};