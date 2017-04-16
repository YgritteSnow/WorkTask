#pragma once

#include "Mesh.h"
#include "Animator.h"

/************************************************************************/
/* 蒙皮网格
/*   持有一个骨骼
/*   如果需要做动作，则在创建时期将骨骼注册给动作及动作管理器
/************************************************************************/
class SkinnedMesh : public Mesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();

public:
	virtual void Load() override;
	virtual void BeforeRender() override;
private:
	// 根据骨骼信息，重新计算 vertex buffer
	void RefreshBySkeleton();

protected:
	// 初始顶点位置
	WorldPos* m_vec_vpos; // 初始顶点位置
	// 动画
	Animator* m_animator;
};