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