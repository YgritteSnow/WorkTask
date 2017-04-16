#pragma once
/**
 * ����
 * ���������ڶ�֡�е�ÿ�ڵ�ı任��Ϣ
**/

#include "JMath.h"
#include "Skeleton.h"
#include "JTime.h"

/************************************************************************/
/* ��Դhandle������
/************************************************************************/
typedef int AniSourceID;
typedef int SkeSourceID;

/************************************************************************/
/* ������Դ��һ�ݣ�
/************************************************************************/
struct AnimateData
{
	AnimateData();
	~AnimateData();

	struct FrameInfo
	{
		FrameInfo();
		~FrameInfo();
		JointData* joints;
		float time;
	};
	FrameInfo* m_frames;	// ����֡
	int m_frames_count;		// �ؼ�֡�ĸ���
	int m_bones_count;		// ��������
	float m_duation;		// ������ʱ��
};

/************************************************************************/
/* ����ʵ������ݣ�
/************************************************************************/
class Animator
{
public:
	Animator(SkeSourceID ske_id, AniSourceID sni_id);
	~Animator();
public:
	void Update(TimeType t);
	WorldPos CalculateVertexPos(int bone_idx, const WorldPos& old_pos); // ת���Թ���������
	
	bool IsDirty() { return m_isDirty; }
	void Cleaned() { m_isDirty = false; }

	void Start();
	void Stop();
private:
	int FindFrameIndex(float t);
	void RefreshJoints();
private:
	SkeSourceID m_ske_id;
	AniSourceID m_ani_id;

	TimeType m_cur_time;		// ��ǰ���ŵ���ʱ��
	float m_duation_scale;		// ʱ������

	JointData* m_cur_joints;	// ��ǰ�Ĺ������ֵ���

	const SkeletonData* m_skeleton;		// ������Ϣ
	const AnimateData* m_animate;			// ������Ϣ
	SkeletonCalculator* m_skeletonCalculator; // ����������

	bool m_isDirty;	// �Ƿ���й���������
	bool m_isActive; // �Ƿ��ڲ���
};