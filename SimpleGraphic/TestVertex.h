#pragma once

#include "ABuffer.h"

struct TestVertex {
	TestVertex() :pos(), color(), normal(0, 0, 1) {}
	TestVertex(float x, float y, float z)
		:pos(x, y, z)
		, color()
		, normal(0, 0, 1)
		, biNormal(1, 0, 0)
	{}
	TestVertex(float x, float y, float z, float r, float g, float b, float a)
		:pos(x, y, z)
		, color(r, g, b, a)
		, normal(0, 0, 1)
		, biNormal(1, 0, 0)
	{}
	TestVertex(float x, float y, float z, const NormColor4& c)
		:pos(x, y, z)
		, color(c)
		, normal(0, 0, 1)
		, biNormal(1, 0, 0)
	{}
	TestVertex(const WorldPos& p, const NormColor4& c, const WorldPos& n)
		:pos(p)
		, color(c)
		, normal(n)
		, biNormal(1, 0, 0)
	{}
	TestVertex(const WorldPos& p, const NormColor4& c, const WorldPos& n, const UVPos& u)
		:pos(p)
		, color(c)
		, normal(n)
		, uv(u)
		, biNormal(1, 0, 0)
	{}
	WorldPos pos;
	NormColor4 color;
	WorldPos normal;
	UVPos uv;
	WorldPos biNormal;	// ���߿ռ�x�᷽��

	explicit operator NormColor4() const { return color; }
	explicit operator ShortColor4() const { return static_cast<ShortColor4>(color); }
	explicit operator AlphaBlendData() const { return AlphaBlendData(color, pos._z); }
};

struct TestVertex_v2p {
	WorldPos pos;		// ��Ļ�ռ�����
	float z;			// �����z���꣨����͸��У����
	NormColor4 color;	// ��ɫ
	WorldPos normal;	// ����
	WorldPos worldPos;	// ����ռ�����
	UVPos uv;			// uv
	WorldPos biNormal;	// ���߿ռ�x�᷽��
	WorldPos taNormal;	// ���߿ռ�x�᷽��
};

struct TestPixel {
	WorldPos pos;
	NormColor4 color;
};