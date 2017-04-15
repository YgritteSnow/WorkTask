#pragma once
/**
 * ����
 * ��Դ�����㺯��
**/
#include "JMath.h"
#include "VertexBone.h"

/************************************************************************/
/* �任��Ϣ
/************************************************************************/
struct JointData
{
	Quaternion quaternion;
	WorldPos translation;
	float scale;
};

/************************************************************************/
/* ������Դ��һ�ݣ�
/************************************************************************/
class SkelentonData
{
public:
	SkelentonData();
	~SkelentonData();
public:
	int m_bones_count;		// ������ĸ���
	JointData* m_joints;	// T-pos�µı任
};

/************************************************************************/
/* ����ʵ������ݣ�
/* �������������ʵʱ����Ϣ
/************************************************************************/
class SkelentonCalculator
{
public:
	SkelentonCalculator(const SkelentonData* s);
	~SkelentonCalculator();
private:
	const SkelentonData* m_skelentonData;	// ������Ϣ
	Matrix44* m_localMat;			// ��ǰÿ�ڵ�任�ľ�����ʽ
};