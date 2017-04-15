#ifndef __MODEL_H__
#define __MODEL_H__

#include "Mesh.h"
#include "ShaderStruct.h"
#include "ShaderManager.h"

class Model{
public:
	Model()
		: m_mesh(nullptr)
		, m_vsMain(nullptr)
		, m_psMain(nullptr)
	{}
public:
	virtual void BeforeRender() {
		ShaderManager::GetInstance()->SetShader(m_vsMain, m_psMain);
	}
	//virtual void Load() { m_mesh->Load(); }

	VertexBuffer* GetVertexBuffer() { return m_mesh->GetVertexBuffer(); }
	IndexBuffer* GetIndexBuffer() { return m_mesh->GetIndexBuffer(); }

	Matrix44 GetModelMat() { return m_worldMat; }
	void RotateXYZ(float x, float y, float z) { m_worldMat = m_worldMat.RotateXYZ(x, y, z); }
	void Translate(float x, float y, float z) { m_worldMat = m_worldMat.Translate(x, y, z); }
	VertexShader* GetVertexShader() { return m_vsMain; }
	PixelShader* GetPixelShader() { return m_psMain; }
protected:
	Matrix44 m_worldMat;// 世界矩阵
	Mesh* m_mesh;// 模型数据

	VertexShader* m_vsMain;
	PixelShader* m_psMain;
};

#endif