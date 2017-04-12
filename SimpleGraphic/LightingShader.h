#pragma once
#include "JMath.h"
#include "ShaderStruct.h"
#include "TestVertex.h"
#include "StructMeta.h"

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
	TexBuffer<NormColor4>* m_cur_texture;
	TexBuffer<NormColor4>* m_cur_normalTexture;
	void SetTexture(TexBuffer<NormColor4>* tex) { m_cur_texture = tex; }
	void SetNormalTexture(TexBuffer<NormColor4>* tex) { m_cur_normalTexture = tex; }
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