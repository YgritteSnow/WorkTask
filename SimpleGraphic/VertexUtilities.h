#pragma once
#include <Windows.h>
#include "JMath.h"

extern unsigned int WINDOW_POS_X;
extern unsigned int WINDOW_POS_Y;
extern unsigned int WINDOW_WIDTH;
extern unsigned int WINDOW_HEIGHT;

// ��Ⱦ״̬�� mask
typedef unsigned char StateMaskType;

#define StateMaskAll				(0 - 1)
#define StateMaskNull				(0)

// ���Ҳȥ����
#define StateMask_Light				(0x1 << 0)
#define StateMaskValue_LightEnable	(0x0 << 0)
#define StateMaskValue_LightDisable (0x1 << 0)

#define StateMask_DrawMode			(0x1 << 1)
#define StateMaskValue_Wareframe	(0x0 << 1)
#define StateMaskValue_Fill			(0x1 << 1)

// �������ȥ����
#define StateMask_CalNormal			(0x1 << 2)
#define StateMaskValue_CalNormal	(0x0 << 2)
#define StateMaskValue_NotCalNormal	(0x1 << 2)

#define StateMask_PerspecCorrect	(0x1 << 3)
#define StateMaskValue_noCorrect	(0x0 << 3)
#define StateMaskValue_withCorrect	(0x1 << 3)

#define StateMask_BackCull			(0x1 << 4)
#define StateMaskValue_BackCullR	(0x0 << 4)
#define StateMaskValue_BackCull		(0x1 << 4)

#define StateMask_DepthBuffer		(0x1 << 5)
#define StateMaskValue_UseDepth		(0x0 << 5)
#define StateMaskValue_NoDepth		(0x1 << 5)

#define StateMask_Alpha				(0x1 << 6)
#define StateMaskValue_NoAlpha		(0x0 << 6)
#define StateMaskValue_UseAlpha		(0x1 << 6)

bool CheckState(StateMaskType curState, StateMaskType state, StateMaskType value);

// �����βü��� mask
typedef unsigned char MaskType;
extern const MaskType NO_CULLED;
extern const MaskType BACK_CULLED;
extern const MaskType SCREEN_CULLED;
inline
MaskType IS_CULLED(MaskType c) { return c ^ NO_CULLED; }



/* *********************************************
* ���ݶ����б����㶥��ķ��ߡ����ߵ�
* *********************************************/
// ���㶥�㷨����
template <typename VertexStruct>
void ProcessVertex_calNormal(VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count) {
	for (size_t vertex_idx = 0; vertex_idx != vertex_count; ++vertex_idx) {
		dst_vertex[vertex_idx].normal.Zero();
	}

	for (size_t indice_idx = 0; indice_idx < indice_count - 2; indice_idx += 3) {
		DWORD vertex_idx_0 = indice[indice_idx];
		DWORD vertex_idx_1 = indice[indice_idx + 1];
		DWORD vertex_idx_2 = indice[indice_idx + 2];

		WorldPos src_v01 = src_vertex[vertex_idx_1].pos - src_vertex[vertex_idx_0].pos;
		WorldPos src_v12 = src_vertex[vertex_idx_2].pos - src_vertex[vertex_idx_1].pos;
		WorldPos src_v20 = src_vertex[vertex_idx_0].pos - src_vertex[vertex_idx_2].pos;

		dst_vertex[vertex_idx_0].normal += src_v20.CrossProduct(src_v01);
		dst_vertex[vertex_idx_1].normal += src_v01.CrossProduct(src_v12);
		dst_vertex[vertex_idx_2].normal += src_v12.CrossProduct(src_v20);
	}

	for (size_t vertex_idx = 0; vertex_idx != vertex_count; ++vertex_idx) {
		dst_vertex[vertex_idx].normal.Normalise();
	}
}

// ���㶥��������
template <typename VertexStruct>
void ProcessVertex_calBiNormal(VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count) {
	for (size_t vertex_idx = 0; vertex_idx != vertex_count; ++vertex_idx) {
		dst_vertex[vertex_idx].biNormal.Zero();
	}

	for (size_t indice_idx = 0; indice_idx < indice_count - 2; indice_idx += 3) {
		DWORD vertex_idx_0 = indice[indice_idx];
		DWORD vertex_idx_1 = indice[indice_idx + 1];
		DWORD vertex_idx_2 = indice[indice_idx + 2];

		WorldPos src_v01 = src_vertex[vertex_idx_1].pos - src_vertex[vertex_idx_0].pos;
		WorldPos src_v02 = src_vertex[vertex_idx_2].pos - src_vertex[vertex_idx_0].pos;

		// ���� delta uv
		auto u01 = src_vertex[vertex_idx_1].uv._x - src_vertex[vertex_idx_0].uv._x;
		auto u02 = src_vertex[vertex_idx_2].uv._x - src_vertex[vertex_idx_0].uv._x;
		auto v01 = src_vertex[vertex_idx_1].uv._y - src_vertex[vertex_idx_0].uv._y;
		auto v02 = src_vertex[vertex_idx_2].uv._y - src_vertex[vertex_idx_0].uv._y;
		// ���� delta uv �������
		// delta_uv :	(  u01,  v01 )
		//				(  u02,  v02 )
		// delta_uv �棺	(  v02, -v01 ) * 1/( u01*v01 - v01*u02 )
		//				( -u02,  u01 )
		auto trace = (u01*v02 - v01*u02);
		u01 /= trace;
		u02 /= trace;
		v01 /= trace;
		v02 /= trace;

		// ����������
		WorldPos biNormal = WorldPos(
			- v02 * src_v01._x + v01 * src_v02._x,
			- v02 * src_v01._y + v01 * src_v02._y,
			- v02 * src_v01._z + v01 * src_v02._z
			);
		
		dst_vertex[vertex_idx_0].biNormal += biNormal;
		dst_vertex[vertex_idx_1].biNormal += biNormal;
		dst_vertex[vertex_idx_2].biNormal += biNormal;
	}

	for (size_t vertex_idx = 0; vertex_idx != vertex_count; ++vertex_idx) {
		dst_vertex[vertex_idx].biNormal.Normalise();
	}
}

/* *********************************************
* ������ɫ��أ�����Ų��shader��ȥ����
* *********************************************/
template <typename VertexStruct>
void ProcessVertex_noShader(VertexStruct* src_vertex, VertexStruct** p_dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count, Matrix44 modelMat, StateMaskType renderState) {
	*p_dst_vertex = new VertexStruct[vertex_count];
	memcpy(*p_dst_vertex, src_vertex, vertex_count * sizeof(VertexStruct));

	WorldPos* p_worldPos = new WorldPos[vertex_count];

	ProcessVertex_pos(src_vertex, *p_dst_vertex, p_worldPos, vertex_count, modelMat);

	ProcessVertex_transNormal(src_vertex, *p_dst_vertex, vertex_count, indice, indice_count, modelMat);
	if (CheckState(renderState, StateMask_Light, StateMaskValue_LightEnable)) {
		ProcessVertex_light(src_vertex, *p_dst_vertex, p_worldPos, vertex_count, modelMat);
	}
}

// ת������λ�õ�����ռ�
template <typename VertexStruct>
void ProcessVertex_pos(VertexStruct* src_vertex, VertexStruct* dst_vertex, WorldPos* worldPos, size_t vertex_count, Matrix44 modelMat) {
	for (size_t idx = 0; idx != vertex_count; ++idx) {
		HomoPos tmp = modelMat.PreMulVec((dst_vertex + idx)->pos.ToVec4Pos());
		worldPos[idx] = tmp.ToVec3Homo();

		auto tmp2 = CameraManager::GetInstance()->CurrentCamera()->GetViewProjMat().PreMulVec(tmp);
		WorldPos screen_pos = CameraManager::GetInstance()->CurrentCamera()->TransToScreenPos(tmp2.ToVec3Homo());
		(dst_vertex + idx)->pos = screen_pos;
	}
}

// ת�����㷨����
template <typename VertexStruct>
void ProcessVertex_transNormal(VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count, Matrix44 modelMat) {
	// ��normalת��������ռ�
	for (size_t vertex_idx = 0; vertex_idx != vertex_count; ++vertex_idx) {
		dst_vertex[vertex_idx].normal.Normalise();
		auto tt = modelMat.PreMulVec(dst_vertex[vertex_idx].normal.ToVec4Dir());
		dst_vertex[vertex_idx].normal = modelMat.PreMulVec(dst_vertex[vertex_idx].normal.ToVec4Dir()).ToVec3();
	}
}

// ���㶥����ɫ
template <typename VertexStruct>
void ProcessVertex_light(VertexStruct* src_vertex, VertexStruct* dst_vertex, WorldPos* worldPos, size_t vertex_count, Matrix44 modelMat) {
	for (size_t vertex_idx = 0; vertex_idx < vertex_count; ++vertex_idx) {
		dst_vertex[vertex_idx].color = LightManager::GetInstance()->Process(
			dst_vertex[vertex_idx].color,
			dst_vertex[vertex_idx].normal,
			worldPos[vertex_idx]);
	}
}

/* *********************************************
* �����βü�
* *********************************************/
MaskType ProcessTriangle_backCull(const WorldPos& v0, const WorldPos& v1, const WorldPos& v2, MaskType curState);
MaskType ProcessTriangle_screenCull(const WorldPos& v0, const WorldPos& v1, const WorldPos& v2);