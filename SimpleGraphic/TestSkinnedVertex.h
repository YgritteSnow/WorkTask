#pragma once

#include "ABuffer.h"
#include "VertexBone.h"

struct TestSkinnedVertex {
	TestSkinnedVertex() :pos(), color(), normal(0, 0, 1) {}
	TestSkinnedVertex(float px, float py, float pz, float u, float v)
		:pos(px, py, pz)
		, uv(u, v)
		, color(1, 1, 1, 1)
		, biNormal(1, 0, 0)
		, normal(1, 0, 0)
		, vertexBone()
	{}
	TestSkinnedVertex(float px, float py, float pz, float u, float v, const VertexBone& b)
		:pos(px, py, pz)
		, uv(u, v)
		, color(1,1,1,1)
		, biNormal(1, 0, 0)
		, normal(1, 0, 0)
		, vertexBone(b)
	{}
	WorldPos pos;
	NormColor4 color;
	WorldPos normal;
	UVPos uv;
	WorldPos biNormal;	// ���߿ռ�x�᷽��
	VertexBone vertexBone; // ��Ƥ��������Ȩ��

	explicit operator NormColor4() const { return color; }
	explicit operator ShortColor4() const { return static_cast<ShortColor4>(color); }
	explicit operator AlphaBlendData() const { return AlphaBlendData(color, pos._z); }
};

struct TestSkinnedVertex_v2p {
	WorldPos pos;		// ��Ļ�ռ�����
	float z;			// �����z���꣨����͸��У����
	NormColor4 color;	// ��ɫ
	WorldPos normal;	// ����
	WorldPos worldPos;	// ����ռ�����
	UVPos uv;			// uv
	WorldPos biNormal;	// ���߿ռ�x�᷽��
	WorldPos taNormal;	// ���߿ռ�x�᷽��
};