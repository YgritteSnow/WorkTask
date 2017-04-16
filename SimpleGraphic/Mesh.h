#pragma once

#include <Windows.h>
#include <vector>
#include <cmath>

#include "JMath.h"
#include "Color.h"
#include "ABuffer.h"
#include "Material.h"
#include "VertexBuffer.h"

/* *********************************************
* Mesh
* *********************************************/

class Mesh {
public:
	Mesh();
	~Mesh();

	// todo ����Ĳ����Ժ�Ӧ�������ṩ��Դ��handle
	virtual void Load();
	virtual void BeforeRender() {}
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();

protected:
	virtual void OnLoad() = 0;
	VertexBuffer* m_vertexBuffer;	// ���㻺��
	IndexBuffer* m_indexBuffer;		// ��������
};