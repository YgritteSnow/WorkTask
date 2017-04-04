#pragma once

#include "ABuffer.h"

struct TestVertex {
	TestVertex() :pos(), color(), normal(0, 0, 1) {}
	TestVertex(float x, float y, float z)
		:pos(x, y, z)
		, color()
		, normal(0, 0, 1)
	{}
	TestVertex(float x, float y, float z, float r, float g, float b, float a)
		:pos(x, y, z)
		, color(r, g, b, a)
		, normal(0, 0, 1)
	{}
	TestVertex(float x, float y, float z, const NormColor4& c)
		:pos(x, y, z)
		, color(c)
		, normal(0, 0, 1)
	{}
	TestVertex(const WorldPos& p, const NormColor4& c, const WorldPos& n)
		:pos(p)
		, color(c)
		, normal(n)
	{}
	TestVertex(const WorldPos& p, const NormColor4& c, const WorldPos& n, const UVPos& u)
		:pos(p)
		, color(c)
		, normal(n)
		, uv(u)
	{}
	WorldPos pos;
	NormColor4 color;
	WorldPos normal;
	UVPos uv;

	explicit operator NormColor4() const { return color; }
	explicit operator ShortColor4() const { return static_cast<ShortColor4>(color); }
	explicit operator AlphaBlendData() const { return AlphaBlendData(color, pos._z); }
};

struct TestVertex_v2p {
	WorldPos pos;
	NormColor4 color;
	UVPos uv;
};

struct TestPixel {
	WorldPos pos;
	NormColor4 color;
};