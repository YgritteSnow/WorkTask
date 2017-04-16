#pragma once
/**
 * 骨骼
 * 资源、计算函数
**/
#include "JMath.h"
#include "VertexBone.h"

/************************************************************************/
/* 变换信息
/************************************************************************/
struct JointData
{
	JointData()
		: parent_index(0)
		, quaternion()
		, translation()
		, scale(WorldPos(1.f, 1.f, 1.f))
	{}
	JointData(int pi, const Quaternion& q, const WorldPos& w, float s)
		: parent_index(pi)
		, quaternion(q)
		, translation(w)
		, scale(WorldPos(s, s, s))
	{}

	int parent_index;
	Quaternion quaternion;
	WorldPos translation;
	WorldPos scale;

	Matrix44 toMatrix44() {
		return quaternion.toMatrix44(translation, scale);
	}
};

/************************************************************************/
/* 骨骼资源（一份）
/************************************************************************/
class SkeletonData
{
public:
	SkeletonData(int bone_count);
	~SkeletonData();
public:
	int m_bones_count;		// 骨骼点的个数
	JointData* m_joints_local;	// T-pos下的变换（局部空间）
};

/************************************************************************/
/* 骨骼计算器（工具型：被动计算）
/*   存储内容：
/*     原始骨骼信息
/*     当前骨骼偏移信息
/*   提供函数：
/*     初始化时处理原始骨骼数据
/*     提供当前骨骼偏移变化后数据更新的函数
/*     提供计算顶点位置的函数
/************************************************************************/
class SkeletonCalculator
{
public:
	SkeletonCalculator(const SkeletonData* s);
	~SkeletonCalculator();
public:
	// 初始化时缓存初始骨骼数据
	void InitSkeleton();
	// 当前骨骼偏移变化后数据更新
	void ChangeSkeleton(JointData* vec_trans);
	// 计算顶点位置
	WorldPos CalculateVertexPos(int bone_idx, const WorldPos& old_pos);
private:
	void _InnerCalMat(const SkeletonData* vec_joints, Matrix44* vec_mats);
	void _InnerCalMatInv(const SkeletonData* skeletonData, Matrix44* vec_mats);
private:
	const SkeletonData* m_skeletonData_origin;	// 骨骼信息
	Matrix44* m_jointsMatInv_origin;				// 初始化之后，预计算 T-pos 下的变换（世界空间）

	SkeletonData* m_skeletonData_cur;	// 当前骨骼信息
	Matrix44* m_jointsMat_cur;		// 当前的各骨骼点的变换
};