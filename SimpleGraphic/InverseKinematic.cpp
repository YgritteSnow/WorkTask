#include "InverseKinematic.h"


InverseKinematic::InverseKinematic(SkeletonData* s)
	: m_skeleton(s)
{
	m_skeletonCalculator = new SkeletonCalculator(s);
	m_jacobian = new SkeletonData(s->m_bones_count);
}

InverseKinematic::~InverseKinematic() {
	delete m_skeletonCalculator;
	m_skeletonCalculator = nullptr;
	delete m_jacobian;
	m_jacobian = nullptr;
}

// ���㵱ǰԼ�������£���������ڸ����ƶ�ֵ�������ܵĹ���
float InverseKinematic::GetDeriv(int bone_idx, float delta) {
}

void InverseKinematic::RefreshJacobian() {
}

void InverseKinematic::ApplySolution() {
}