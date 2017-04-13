#pragma once

#include "Mesh.h"
#include "Skeleton.h"
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
	Skeleton* m_skeleton;
	Animator* m_animator;
};