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
	Quaternion quaternion;
	WorldPos translation;
	float scale;
};

/************************************************************************/
/* 骨骼资源（一份）
/************************************************************************/
class SkelentonData
{
public:
	SkelentonData();
	~SkelentonData();
public:
	int m_bones_count;		// 骨骼点的个数
	JointData* m_joints;	// T-pos下的变换
};

/************************************************************************/
/* 骨骼实例（多份）
/* 包含计算所需的实时的信息
/************************************************************************/
class SkelentonCalculator
{
public:
	SkelentonCalculator(const SkelentonData* s);
	~SkelentonCalculator();
private:
	const SkelentonData* m_skelentonData;	// 骨骼信息
	Matrix44* m_localMat;			// 当前每节点变换的矩阵形式
};