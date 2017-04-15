#pragma once
/**
 * ����
 * ���������ڶ�֡�е�ÿ�ڵ�ı任��Ϣ
**/

#include "JMath.h"
#include "Skelenton.h"

/************************************************************************/
/* ������Դ��һ�ݣ�
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
	//SkelentonData* m_skeleton;	// Ҫ�������Ĺ���
	FrameInfo* m_frames;	// ����֡
	int m_frames_count;		// �ؼ�֡�ĸ���
	int m_bones_count;		// ��������
};

/************************************************************************/
/* ����ʵ������ݣ�
/************************************************************************/
class Animator
{
public:
	Animator();
	~Animator();
private:
	AnimateData* m_animate;			// ������Ϣ
	SkelentonCalculator* m_skeletonCalculator; // �����������
};