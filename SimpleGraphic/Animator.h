#pragma once

#include "JMath.h"
#include "Skeleton.h"

struct JointInfo
{
	HomoPos quaternion;
	WorldPos translation;
	float scale;
};

struct FrameInfo
{
	JointInfo* joints;
	float time;
};

class Animator
{
private:
	Skeleton* m_skeleton;	// 要做动作的骨骼
	FrameInfo* m_frames;	// 动画帧
	int m_frames_count;		// 关键帧的个数
};