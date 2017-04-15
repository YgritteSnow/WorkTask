#include "Skelenton.h"


/************************************************************************/
/* ������Դ��һ�ݣ�
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
/* ����ʵ������ݣ�
/* �������������ʵʱ����Ϣ
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