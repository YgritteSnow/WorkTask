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
	WorldPos biNormal;	// 切线空间x轴方向
	VertexBone vertexBone; // 蒙皮骨骼及其权重

	explicit operator NormColor4() const { return color; }
	explicit operator ShortColor4() const { return static_cast<ShortColor4>(color); }
	explicit operator AlphaBlendData() const { return AlphaBlendData(color, pos._z); }
};

struct TestSkinnedVertex_v2p {
	WorldPos pos;		// 屏幕空间坐标
	float z;			// 最初的z坐标（用于透视校正）
	NormColor4 color;	// 颜色
	WorldPos normal;	// 法线
	WorldPos worldPos;	// 世界空间坐标
	UVPos uv;			// uv
	WorldPos biNormal;	// 切线空间x轴方向
	WorldPos taNormal;	// 切线空间x轴方向
};