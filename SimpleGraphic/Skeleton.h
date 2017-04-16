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
/* ������Դ��һ�ݣ�
/************************************************************************/
class SkeletonData
{
public:
	SkeletonData(int bone_count);
	~SkeletonData();
public:
	int m_bones_count;		// ������ĸ���
	JointData* m_joints_local;	// T-pos�µı任���ֲ��ռ䣩
};

/************************************************************************/
/* �����������������ͣ��������㣩
/*   �洢���ݣ�
/*     ԭʼ������Ϣ
/*     ��ǰ����ƫ����Ϣ
/*   �ṩ������
/*     ��ʼ��ʱ����ԭʼ��������
/*     �ṩ��ǰ����ƫ�Ʊ仯�����ݸ��µĺ���
/*     �ṩ���㶥��λ�õĺ���
/************************************************************************/
class SkeletonCalculator
{
public:
	SkeletonCalculator(const SkeletonData* s);
	~SkeletonCalculator();
public:
	// ��ʼ��ʱ�����ʼ��������
	void InitSkeleton();
	// ��ǰ����ƫ�Ʊ仯�����ݸ���
	void ChangeSkeleton(JointData* vec_trans);
	// ���㶥��λ��
	WorldPos CalculateVertexPos(int bone_idx, const WorldPos& old_pos);
private:
	// Local Space -> Model Space
	void _InnerTransSQTToModelSpace(const SkeletonData* joints_old, SkeletonData* joints_new);
	// SQT -> Mat
	void _InnerSQT2Mat(const SkeletonData* vec_joints, Matrix44* vec_mats);
	void _InnerSQT2MatInv(const SkeletonData* joints_old, Matrix44* vec_mats);
private:
	const SkeletonData* m_sqt_origin_local;		// SQT��ʽ - ԭʼ - �����ռ� - ��ȡ��

	SkeletonData* m_sqt_curent_local;			// SQT��ʽ - ԭʼ - �����ռ� - ��ȡ��
	Matrix44*	  m_mat_origin_model_inv;		// �����ʽ - ԭʼ - ģ�Ϳռ� - ȡ��

	SkeletonData* m_sqt_curent_model;			// SQT��ʽ - ��ǰ - ģ�Ϳռ� - ��ȡ��
	Matrix44*	  m_mat_curent_model;			// �����ʽ - ��ǰ - ģ�Ϳռ� - ��ȡ��
};