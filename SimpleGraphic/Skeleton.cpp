#include "Skeleton.h"

#include <assert.h>

/************************************************************************/
/* 骨骼资源（一份）
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
/* 骨骼实例（多份）
/* 包含计算所需的实时的信息
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

// 按照 father - child 的方式，把骨骼点的偏移添加上去
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
 骨骼的计算：
	1. 根据骨骼点的初始位置，将顶点的初始位置转换到骨骼空间中
	2. 在骨骼空间，根据骨骼变换，计算新的位置
	3. 将新的位置转换回局部空间
	（1 中的两个数值都是固定的，可以考虑初始化的时候缓存一下。因为数据肯定是存在顶点，
		又涉及多个骨骼，故而应该在顶点范围内划定长度为4个WorldPos的缓存。以后可以实
		现试试~）
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