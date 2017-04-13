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
	// 根据骨骼情况，更新顶点缓存
	void RefreshVertexBuffer();

private:
	Skeleton* m_skeleton;
	Animator* m_animator;
};