#pragma once

#include "Skeleton.h"

/**
 * 反向运动学
 *   功能：将骨骼的某个点（尽力）指向某个方向/接近某个位置
 *   生效时期：骨骼动画的运算结束后即刻进行
**/

/************************************************************************/
/* IK要解决的目标
/************************************************************************/
struct IKTarget
{
	enum IKTargetType {
		Direction,
		Position,
	};
	IKTargetType type;
	WorldPos target_data;
};

class InverseKinematic
{
public:
	InverseKinematic(SkeletonData* s);
	~InverseKinematic();

private:
	// 计算当前约束条件下，骨骼点对于给定移动值的最大可能的贡献
	float GetDeriv(int bone_idx, float delta);
	void RefreshJacobian();
	void ApplySolution();

private:
	IKTarget m_target;			// 要解决的任务
	SkeletonData* m_skeleton;	// 将被修改的骨骼
	SkeletonData* m_jacobian;	// 对目标的贡献能力（一阶导）

	// 暂时不支持对骨骼的约束 todo 因为还没想到做的方案
	// BoneConstrait* m_constraits; // 骨骼的约束器

	SkeletonCalculator* m_skeletonCalculator; // 辅助计算
};