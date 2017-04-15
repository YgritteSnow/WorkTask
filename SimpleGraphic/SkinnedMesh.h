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
	// 根据骨骼情况，更新顶点缓存
	void RefreshVertexBuffer();

private:
	Animator* m_animator;
};