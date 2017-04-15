#pragma once
/**
 * 顶点属性 - 骨骼绑定
 * 描述顶点绑定到的骨骼点及其权重
 * 最多绑定到 4 个骨骼点
**/
const int MAX_BONE = 4;

struct VertexBone
{
	VertexBone() {
		bone_count = 0;
	}
	VertexBone(int b0, float b0w){
		bone_count = 1;
		bone_index[0] = b0;
		bone_weight[0] = b0w;
	}
	VertexBone(int b0, float b0w, int b1, float b1w) {
		bone_count = 2;
		bone_index[0] = b0;
		bone_weight[0] = b0w;
		bone_index[1] = b1;
		bone_weight[1] = b1w;
	}
	VertexBone(int b0, float b0w, int b1, float b1w, int b2, float b2w) {
		bone_count = 3;
		bone_index[0] = b0;
		bone_weight[0] = b0w;
		bone_index[1] = b1;
		bone_weight[1] = b1w;
		bone_index[2] = b2;
		bone_weight[2] = b2w;
	}
	VertexBone(int b0, float b0w, int b1, float b1w, int b2, float b2w, int b3, float b3w) {
		bone_count = 4;
		bone_index[0] = b0;
		bone_weight[0] = b0w;
		bone_index[1] = b1;
		bone_weight[1] = b1w;
		bone_index[2] = b2;
		bone_weight[2] = b2w;
		bone_index[3] = b3;
		bone_weight[3] = b3w;
	}
	int bone_index[MAX_BONE];
	float bone_weight[MAX_BONE];
	int bone_count;
};