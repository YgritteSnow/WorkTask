#ifndef __MODEL_H__
#define __MODEL_H__

#include "Mesh.h"
#include "ShaderStruct.h"

class Model{
public:
	Model()
		: m_mesh(nullptr)
		, m_vsID(0)
		, m_psID(0)
	{}
public:
	//virtual void Load() { m_mesh->Load(); }

	VertexBuffer* GetVertexBuffer() { return m_mesh->GetVertexBuffer(); }
	IndexBuffer* GetIndexBuffer() { return m_mesh->GetIndexBuffer(); }

	JMath::Mat44 GetModelMat() { return m_worldMat; }
	void RotateXYZ(float x, float y, float z) { m_worldMat = m_worldMat.RotateXYZ(x, y, z); }
	void Translate(float x, float y, float z) { m_worldMat = m_worldMat.Translate(x, y, z); }
	ShaderID GetVertexShader() { return m_vsID; }
	ShaderID GetPixelShader() { return m_psID; }
protected:
	JMath::Mat44 m_worldMat;// 世界矩阵
	Mesh* m_mesh;// 模型数据
	ShaderID m_vsID;
	ShaderID m_psID;
};

#endif