#include "Skeleton.h"

#include <assert.h>

/************************************************************************/
/* ������Դ��һ�ݣ�
/************************************************************************/
SkeletonData::SkeletonData(int bone_count)
	: m_joints_local(nullptr)
	, m_bones_count(bone_count)
{
	m_joints_local = new JointData[bone_count];
}

SkeletonData::~SkeletonData() {
	delete[] m_joints_local;
	m_joints_local = nullptr;
	m_bones_count = 0;
}

/************************************************************************/
/* ����ʵ������ݣ�
/* �������������ʵʱ����Ϣ
/************************************************************************/
SkeletonCalculator::SkeletonCalculator(const SkeletonData* s)
	: m_skeletonData_origin(s)
	, m_jointsMatInv_origin(nullptr)
	, m_skeletonData_cur(nullptr)
	, m_jointsMat_cur(nullptr)
{
	assert(m_skeletonData_origin->m_bones_count > 0);
	m_skeletonData_cur = new SkeletonData(m_skeletonData_origin->m_bones_count);
	m_jointsMatInv_origin = new Matrix44[m_skeletonData_origin->m_bones_count];
	m_jointsMatInv_origin[0].SetIdentity();
	m_jointsMat_cur = new Matrix44[m_skeletonData_origin->m_bones_count];
	m_jointsMat_cur[0].SetIdentity();
	InitSkeleton();
}

SkeletonCalculator::~SkeletonCalculator() {
	delete[] m_jointsMatInv_origin;
	m_jointsMatInv_origin = nullptr;

	delete m_skeletonData_cur;
	m_skeletonData_cur = nullptr;

	delete[] m_jointsMat_cur;
	m_jointsMat_cur = nullptr;
}

void SkeletonCalculator::InitSkeleton() {
	_InnerCalMatInv(m_skeletonData_origin, m_jointsMatInv_origin);
}

void SkeletonCalculator::ChangeSkeleton(JointData* vec_trans){
	for (int i = 0; i < m_skeletonData_origin->m_bones_count; ++i) {
		const auto& src = m_skeletonData_origin->m_joints_local[i];
		const auto& off = vec_trans[i];
		auto& dst = m_skeletonData_cur->m_joints_local[i];
		dst.quaternion = src.quaternion * off.quaternion;
		dst.scale = src.scale * off.scale;
		dst.translation = src.translation + off.translation;
	}
	_InnerCalMat(m_skeletonData_cur, m_jointsMat_cur);
}

/*
 �����ļ��㣺
	1. ���ݹ�����ĳ�ʼλ�ã�������ĳ�ʼλ��ת���������ռ���
	2. �ڹ����ռ䣬���ݹ����任�������µ�λ��
	3. ���µ�λ��ת���ؾֲ��ռ�
	��1 �е�������ֵ���ǹ̶��ģ����Կ��ǳ�ʼ����ʱ�򻺴�һ�¡���Ϊ���ݿ϶��Ǵ��ڶ��㣬
		���漰����������ʶ�Ӧ���ڶ��㷶Χ�ڻ�������Ϊ4��WorldPos�Ļ��档�Ժ����ʵ
		������~��
 */
WorldPos SkeletonCalculator::CalculateVertexPos(int bone_idx, const WorldPos& old_pos) {
	auto localpos = m_jointsMatInv_origin[bone_idx].PreMulVec(old_pos.ToVec4Pos());
	return m_jointsMat_cur[bone_idx].PreMulVec(localpos).ToVec3();
}

// ��ÿ��������任��ģ������ϵ
void SkeletonCalculator::_InnerCalMat(const SkeletonData* skeletonData, Matrix44* vec_mats) {
	for (int i = 0; i < skeletonData->m_bones_count; ++i) {
		if (i == 0) {
			vec_mats[i].SetIdentity();
		}
		else {
			Matrix44 cur_mat = skeletonData->m_joints_local[i].toMatrix44();
			auto& parent_mat = vec_mats[skeletonData->m_joints_local[i].parent_index];
			vec_mats[i] = parent_mat.PreMulMat(cur_mat);
		}
	}
}

// ��ÿ��������任��ģ������ϵ����ȡ��
void SkeletonCalculator::_InnerCalMatInv(const SkeletonData* skeletonData, Matrix44* vec_mats) {
	for (int i = 0; i < skeletonData->m_bones_count; ++i) {
		if (i == 0) {
			vec_mats[i].SetIdentity();
		}
		else {
			Matrix44 cur_mat = skeletonData->m_joints_local[i].toMatrix44();
			auto& parent_mat = vec_mats[skeletonData->m_joints_local[i].parent_index];
			vec_mats[i] = parent_mat.PreMulMat(cur_mat);
			vec_mats[i] = vec_mats[i].Inverse();
		}
	}
}