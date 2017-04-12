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
	WorldPos biNormal;	// 切线空间x轴方向

	explicit operator NormColor4() const { return color; }
	explicit operator ShortColor4() const { return static_cast<ShortColor4>(color); }
	explicit operator AlphaBlendData() const { return AlphaBlendData(color, pos._z); }
};

struct TestVertex_v2p {
	WorldPos pos;		// 屏幕空间坐标
	float z;			// 最初的z坐标（用于透视校正）
	NormColor4 color;	// 颜色
	WorldPos normal;	// 法线
	WorldPos worldPos;	// 世界空间坐标
	UVPos uv;			// uv
	WorldPos biNormal;	// 切线空间x轴方向
	WorldPos taNormal;	// 切线空间x轴方向
};

struct TestPixel {
	WorldPos pos;
	NormColor4 color;
};