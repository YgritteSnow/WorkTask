#pragma once

#include "Skeleton.h"

/**
 * �����˶�ѧ
 *   ���ܣ���������ĳ���㣨������ָ��ĳ������/�ӽ�ĳ��λ��
 *   ��Чʱ�ڣ�������������������󼴿̽���
**/

/************************************************************************/
/* IKҪ�����Ŀ��
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
	// ���㵱ǰԼ�������£���������ڸ����ƶ�ֵ�������ܵĹ���
	float GetDeriv(int bone_idx, float delta);
	void RefreshJacobian();
	void ApplySolution();

private:
	IKTarget m_target;			// Ҫ���������
	SkeletonData* m_skeleton;	// �����޸ĵĹ���
	SkeletonData* m_jacobian;	// ��Ŀ��Ĺ���������һ�׵���

	// ��ʱ��֧�ֶԹ�����Լ�� todo ��Ϊ��û�뵽���ķ���
	// BoneConstrait* m_constraits; // ������Լ����

	SkeletonCalculator* m_skeletonCalculator; // ��������
};