#pragma once
#include "JMath.h"
#include "ShaderStruct.h"
#include "TestVertex.h"
#include "StructMeta.h"
#include "Light.h"

const int MAX_LIGHT = 5;

class TestVSShader : public VertexShader
{
public:
	JMath::Mat44 modelMat;
	void SetModelMat(const JMath::Mat44& mat) { modelMat = mat; }

public:
	DECLARE_VERTEXSHADER(TestVertex, pVin, TestVertex_v2p, pVout);
};

class TestPSShader : public PixelShader
{
public:
	TestPSShader()
		: m_cur_texture(nullptr)
		, m_cur_normalTexture(nullptr)
		, cur_light_count(0)
	{}
	void SetDiffuseTexture(TexBuffer<NormColor4>* tex) { m_cur_texture = tex; }
	void SetNormalTexture(TexBuffer<NormColor4>* tex) { m_cur_normalTexture = tex; }
	void AddLight(Light* l) {
		if (cur_light_count == MAX_LIGHT)
			return;
		m_vec_light[cur_light_count] = l;
		cur_light_count++;
	}
	void SetMaterial(Material* m) { m_material = m; }
public:
	TexBuffer<NormColor4>* m_cur_texture;
	TexBuffer<NormColor4>* m_cur_normalTexture;
	Light* m_vec_light[MAX_LIGHT];
	int cur_light_count;
	Material* m_material;
public:
	DECLARE_PIXELSHADER(TestVertex_v2p, pVout, TestPixel, pPout);
};

//class TestVSShader : public VertexShader
//{
//	DECLARE_VERTEXSHADER(TestVertex, pVin, TestVertex_v2p, pVout);
//};
//
//class TestPSShader : public PixelShader
//{
//	DECLARE_PIXELSHADER(TestVertex_v2p, pVout, TestPixel, pPout);
//};