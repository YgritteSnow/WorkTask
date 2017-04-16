#pragma once

#include "Mesh.h"
#include "Animator.h"

/************************************************************************/
/* ��Ƥ����
/*   ����һ������
/*   �����Ҫ�����������ڴ���ʱ�ڽ�����ע�������������������
/************************************************************************/
class SkinnedMesh : public Mesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();

public:
	virtual void Load() override;
	virtual void BeforeRender() override;
private:
	// ���ݹ�����Ϣ�����¼��� vertex buffer
	void RefreshBySkeleton();

protected:
	// ��ʼ����λ��
	WorldPos* m_vec_vpos; // ��ʼ����λ��
	// ����
	Animator* m_animator;
};