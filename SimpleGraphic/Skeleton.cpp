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
 骨骼的计算：
	1. 根据骨骼点的初始位置，将顶点的初始位置转换到骨骼空间中
	2. 在骨骼空间，根据骨骼变换，计算新的位置
	3. 将新的位置转换回局部空间
	（1 中的两个数值都是固定的，可以考虑初始化的时候缓存一下。因为数据肯定是存在顶点，
		又涉及多个骨骼，故而应该在顶点范围内划定长度为4个WorldPos的缓存。以后可以实
		现试试~）
 */
WorldPos SkeletonCalculator::CalculateVertexPos(int bone_idx, const WorldPos& old_pos) {
	auto localpos = m_jointsMatInv_origin[bone_idx].PreMulVec(old_pos.ToVec4Pos());
	return m_jointsMat_cur[bone_idx].PreMulVec(localpos).ToVec3();
}

// 将每个骨骼点变换到模型坐标系
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

// 将每个骨骼点变换到模型坐标系，并取逆
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