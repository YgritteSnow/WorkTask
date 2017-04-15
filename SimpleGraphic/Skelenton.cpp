#include "Skelenton.h"


/************************************************************************/
/* 骨骼资源（一份）
/************************************************************************/
SkelentonData::SkelentonData()
	: m_joints(nullptr)
	, m_bones_count(0)
{}

SkelentonData::~SkelentonData() {
	delete[] m_joints;
	m_joints = nullptr;
	m_bones_count = 0;
}

/************************************************************************/
/* 骨骼实例（多份）
/* 包含计算所需的实时的信息
/************************************************************************/
SkelentonCalculator::SkelentonCalculator(const SkelentonData* s)
	: m_skelentonData(s)
	, m_localMat(nullptr)
{
	m_localMat = new Matrix44[s->m_bones_count];
}

SkelentonCalculator::~SkelentonCalculator() {
	delete[] m_localMat;
	m_localMat = nullptr;
}