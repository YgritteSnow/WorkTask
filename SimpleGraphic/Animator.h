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
	Skeleton* m_skeleton;	// Ҫ�������Ĺ���
	FrameInfo* m_frames;	// ����֡
	int m_frames_count;		// �ؼ�֡�ĸ���
};