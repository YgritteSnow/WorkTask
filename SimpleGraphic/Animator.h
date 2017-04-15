#pragma once
/**
 * 动画
 * 描述骨骼在多帧中的每节点的变换信息
**/

#include "JMath.h"
#include "Skelenton.h"

/************************************************************************/
/* 动画资源（一份）
/************************************************************************/
class AnimateData
{
public:
	AnimateData();
	~AnimateData();
private:
	struct FrameInfo
	{
		FrameInfo() :joints(nullptr), time(0.f) {}
		~FrameInfo() { delete[] joints; joints = nullptr; }
		JointData* joints;
		float time;
	};
	//SkelentonData* m_skeleton;	// 要做动作的骨骼
	FrameInfo* m_frames;	// 动画帧
	int m_frames_count;		// 关键帧的个数
	int m_bones_count;		// 骨骼个数
};

/************************************************************************/
/* 动画实例（多份）
/************************************************************************/
class Animator
{
public:
	Animator();
	~Animator();
private:
	AnimateData* m_animate;			// 动画信息
	SkelentonCalculator* m_skeletonCalculator; // 骨骼及其计算
};