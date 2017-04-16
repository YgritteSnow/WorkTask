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
	: m_sqt_origin_local(s)
	, m_sqt_curent_local(nullptr)
	, m_mat_origin_model_inv(nullptr)
	, m_sqt_curent_model(nullptr)
	, m_mat_curent_model(nullptr)
{
	assert(m_sqt_origin_local->m_bones_count > 0);
	m_mat_origin_model_inv = new Matrix44[m_sqt_origin_local->m_bones_count];
	m_sqt_curent_local = new SkeletonData(m_sqt_origin_local->m_bones_count);
	m_sqt_curent_model = new SkeletonData(m_sqt_origin_local->m_bones_count);
	m_mat_curent_model = new Matrix44[m_sqt_origin_local->m_bones_count];
	InitSkeleton();
}

SkeletonCalculator::~SkeletonCalculator() {
	delete m_sqt_curent_local;
	m_sqt_curent_local = nullptr;

	delete[] m_mat_origin_model_inv;
	m_mat_origin_model_inv = nullptr;

	delete m_sqt_curent_model;
	m_sqt_curent_model = nullptr;

	delete[] m_mat_curent_model;
	m_mat_curent_model = nullptr;
}

void SkeletonCalculator::InitSkeleton() {
	_InnerTransSQTToModelSpace(m_sqt_origin_local, m_sqt_curent_model);
	_InnerSQT2MatInv(m_sqt_curent_model, m_mat_origin_model_inv);
}

// ���� father - child �ķ�ʽ���ѹ������ƫ�������ȥ
void SkeletonCalculator::ChangeSkeleton(JointData* vec_trans){
	for (int i = 0; i < m_sqt_origin_local->m_bones_count; ++i) {
		const auto& father = m_sqt_origin_local->m_joints_local[i];
		const auto& child = vec_trans[i];

		auto& dst = m_sqt_curent_local->m_joints_local[i];
		dst.parent_index = father.parent_index;
		dst.quaternion = father.quaternion * child.quaternion;
		dst.scale = father.scale * child.scale;
		dst.translation = father.translation + father.quaternion.RotateVec(child.translation);

		int ttt=2;
	}
	_InnerTransSQTToModelSpace(m_sqt_curent_local, m_sqt_curent_model);
	_InnerSQT2Mat(m_sqt_curent_model, m_mat_curent_model);
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
	auto localpos = m_mat_origin_model_inv[bone_idx].PreMulVec(old_pos.ToVec4Pos());
	return m_mat_curent_model[bone_idx].PreMulVec(localpos).ToVec3();
}

// ModelSpace: SQT->MatrixInv
void SkeletonCalculator::_InnerSQT2MatInv(const SkeletonData* skeletonData, Matrix44* vec_mats) {
	for (int i = 0; i < skeletonData->m_bones_count; ++i) {
		JointData tmp = skeletonData->m_joints_local[i];
		tmp.quaternion = tmp.quaternion.Conjugate();
		tmp.scale = 1 / tmp.scale;
		tmp.translation = -tmp.translation;

		vec_mats[i] = tmp.toMatrix44();
	}
}

// SQT: BoneSpace->ModelSpace
void SkeletonCalculator::_InnerTransSQTToModelSpace(const SkeletonData* skeletonData, SkeletonData* joints_new) {
	for (int i = 0; i < skeletonData->m_bones_count; ++i) {
		if (i == 0) {
			joints_new->m_joints_local[i] = skeletonData->m_joints_local[i];
		}
		else {
			auto& dst = joints_new->m_joints_local[i];
			auto& child = skeletonData->m_joints_local[i];
			auto& father = joints_new->m_joints_local[child.parent_index];

			dst.parent_index = child.parent_index;
			dst.quaternion = child.quaternion * father.quaternion;
			dst.translation = father.quaternion.RotateVec(child.translation) + father.translation;
			dst.scale = child.scale * father.scale;
		}
	}
}

// ModelSpace: SQT->Matrix
void SkeletonCalculator::_InnerSQT2Mat(const SkeletonData* skeletonData, Matrix44* vec_mats) {
	for (int i = 0; i < skeletonData->m_bones_count; ++i) {
		vec_mats[i] = skeletonData->m_joints_local[i].toMatrix44();
	}
}