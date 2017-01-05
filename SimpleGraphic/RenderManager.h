#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include <Windows.h>
#include "JMath.h"
#include "SimpleBrush.h"
#include "Light.h"

typedef float DepthBufferPixel;
extern UINT WINDOW_WIDTH;
extern UINT WINDOW_HEIGHT;

typedef unsigned char MaskType;
const MaskType NO_CULLED = 0xff;
const MaskType BACK_CULLED = 0xff ^ 0x01;
const MaskType SCREEN_CULLED = 0xff ^ 0x02;
inline
MaskType IS_CULLED(MaskType c){ return c ^ NO_CULLED; }

class RenderManager{
public:
	// 初始化/析构
	static bool Init(ScreenCoord width, ScreenCoord height);
	static void UnInit();
	static bool IsInited();
	// 获得实例
	static RenderManager* GetInstance();

private:
	// 建立缓存
	bool SetupBuffer(ScreenCoord width, ScreenCoord height);
	// 释放缓存
	void ReleaseBuffer();
	// 设置贴图 （todo 直接使用外部指针，可能变成野指针）
	void SetTexture(ImgBuffer<NormColor4>* tex);
	ImgBuffer<NormColor4>* GetTexture(){ return m_cur_texture; }

public:
	// 清除缓存
	void Clear();
	// 渲染缓存
	template <typename VertexStruct>
	void RenderVertexIndice(VertexStruct* vertex, size_t vertex_count, DWORD* indice, size_t indice_count, JMath::Mat44 modelMat){
		VertexStruct* dst_vertex = nullptr;
		ProcessVertex(vertex, &dst_vertex, vertex_count, indice, indice_count, modelMat);
		MaskType* cull_mask = nullptr;
		ProcessTriangle(&cull_mask, vertex, dst_vertex, vertex_count, indice, indice_count, modelMat);

		for (size_t idx = 0; idx < indice_count - 2; idx+=3){
			if (IS_CULLED(cull_mask[idx / 3])){
				continue;
			}
			size_t indice_idx_0 = indice[idx];
			size_t indice_idx_1 = indice[idx + 1];
			size_t indice_idx_2 = indice[idx + 2];

			const auto& v0 = *(dst_vertex + indice_idx_0);
			const auto& v1 = *(dst_vertex + indice_idx_1);
			const auto& v2 = *(dst_vertex + indice_idx_2);

			//RenderTriangle_wireframe(v0, v1, v2);
			RenderTriangle_fill(v0, v1, v2);
		}

		delete[] dst_vertex;
		delete[] cull_mask;
	}

	void RenderDummy();
	// 提交缓存
	void Present();

private:
	/* *********************************************
	* 对顶点进行处理
	* *********************************************/
	template <typename VertexStruct>
	void ProcessVertex(VertexStruct* src_vertex, VertexStruct** p_dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count, JMath::Mat44 modelMat){
		*p_dst_vertex = new VertexStruct[vertex_count];
		memcpy(*p_dst_vertex, src_vertex, vertex_count * sizeof(VertexStruct));

		WorldPos* p_worldPos = new WorldPos[vertex_count];

		ProcessVertex_pos(src_vertex, *p_dst_vertex, p_worldPos, vertex_count, modelMat);
		// todo 这里其实假定了顶点struct已经有了normal这个属性了，毕竟都把计算结果直接写在人家的normal属性里了。。。
		// 以后优化的时候可以根据输入条件申请更合适的内存结构来储存数据
		//ProcessVertex_calNormal(src_vertex, *p_dst_vertex, vertex_count, indice, indice_count, modelMat);
		ProcessVertex_transNormal(src_vertex, *p_dst_vertex, vertex_count, indice, indice_count, modelMat);
		ProcessVertex_light(src_vertex, *p_dst_vertex, p_worldPos, vertex_count, modelMat);
	}
	// 转换顶点位置到世界空间
	template <typename VertexStruct>
	void ProcessVertex_pos(VertexStruct* src_vertex, VertexStruct* dst_vertex, WorldPos* worldPos, size_t vertex_count, JMath::Mat44 modelMat){
		for (size_t idx = 0; idx != vertex_count; ++idx){
			HomoPos tmp = modelMat.PreMulVec((dst_vertex + idx)->pos.ToVec4Pos());
			worldPos[idx] = tmp.ToVec3Homo();

			tmp = CameraManager::GetInstance()->CurrentCamera()->GetViewProjMat().PreMulVec(tmp);
			WorldPos screen_pos = CameraManager::GetInstance()->CurrentCamera()->GetViewPortPos(tmp.ToVec3Homo());
			(dst_vertex + idx)->pos = screen_pos;
		}
	}
	// 计算顶点法向量
	template <typename VertexStruct>
	void ProcessVertex_calNormal(VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count, JMath::Mat44 modelMat){
		for (size_t vertex_idx = 0; vertex_idx != vertex_count; ++vertex_idx){
			dst_vertex[vertex_idx].normal.Zero();
		}

		for (size_t indice_idx = 0; indice_idx < indice_count - 2; indice_idx += 3){
			DWORD vertex_idx_0 = indice[indice_idx];
			DWORD vertex_idx_1 = indice[indice_idx+1];
			DWORD vertex_idx_2 = indice[indice_idx+2];

			WorldPos src_v01 = src_vertex[vertex_idx_1].pos - src_vertex[vertex_idx_0].pos;
			WorldPos src_v12 = src_vertex[vertex_idx_2].pos - src_vertex[vertex_idx_1].pos;
			WorldPos src_v20 = src_vertex[vertex_idx_0].pos - src_vertex[vertex_idx_2].pos;

			dst_vertex[vertex_idx_0].normal += src_v20.CrossProduct(src_v01);
			dst_vertex[vertex_idx_1].normal += src_v01.CrossProduct(src_v12);
			dst_vertex[vertex_idx_2].normal += src_v12.CrossProduct(src_v20);
		}

	}
	// 转换顶点法向量
	template <typename VertexStruct>
	void ProcessVertex_transNormal(VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count, JMath::Mat44 modelMat){
		// 把normal转换到世界空间
		for (size_t vertex_idx = 0; vertex_idx != vertex_count; ++vertex_idx){
			dst_vertex[vertex_idx].normal.Normalise();
			auto tt = modelMat.PreMulVec(dst_vertex[vertex_idx].normal.ToVec4Dir());
			dst_vertex[vertex_idx].normal = modelMat.PreMulVec(dst_vertex[vertex_idx].normal.ToVec4Dir()).ToVec3();
		}
	}
	// 计算顶点颜色
	template <typename VertexStruct>
	void ProcessVertex_light(VertexStruct* src_vertex, VertexStruct* dst_vertex, WorldPos* worldPos, size_t vertex_count, JMath::Mat44 modelMat){
		for (size_t vertex_idx = 0; vertex_idx < vertex_count; ++vertex_idx){
			dst_vertex[vertex_idx].color = LightManager::GetInstance()->Process(
				dst_vertex[vertex_idx].color,
				dst_vertex[vertex_idx].normal,
				worldPos[vertex_idx]);
		}
	}

	/* *********************************************
	* 对三角形进行处理
	* *********************************************/
	template <typename VertexStruct>
	void ProcessTriangle(MaskType** p_cull_mask,
		VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count, JMath::Mat44 modelMat){
		size_t mask_size = indice_count / 3;
		*p_cull_mask = new MaskType[mask_size]; // 记录三角形是否被裁剪掉
		memset(*p_cull_mask, NO_CULLED, sizeof(MaskType)* mask_size);

		for (size_t idx = 0; idx < indice_count - 2; idx += 3){
			size_t indice_idx_0 = indice[idx];
			size_t indice_idx_1 = indice[idx + 1];
			size_t indice_idx_2 = indice[idx + 2];

			const auto& v0 = *(dst_vertex + indice_idx_0);
			const auto& v1 = *(dst_vertex + indice_idx_1);
			const auto& v2 = *(dst_vertex + indice_idx_2);

			(*p_cull_mask)[idx / 3] &= ProcessTriangle_backCull(v0.pos, v1.pos, v2.pos);
			(*p_cull_mask)[idx / 3] &= ProcessTriangle_screenCull(v0.pos, v1.pos, v2.pos);
		}
	}

	MaskType ProcessTriangle_backCull(WorldPos v0, WorldPos v1, WorldPos v2){
		if ((v1 - v0).CrossProduct(v2 - v1).DotProduct(WorldPos(0, 0, -1)) > 0){
			return BACK_CULLED;
		}
		else{
			return NO_CULLED;
		}
	}
	MaskType ProcessTriangle_screenCull(WorldPos v0, WorldPos v1, WorldPos v2){
		if ((v0._x < 0 || v0._y < 0 || v0._x >= WINDOW_WIDTH || v0._y >= WINDOW_HEIGHT)
			&& (v1._x < 0 || v1._y < 0 || v1._x >= WINDOW_WIDTH || v1._y >= WINDOW_HEIGHT)
			&& (v2._x < 0 || v2._y < 0 || v2._x >= WINDOW_WIDTH || v2._y >= WINDOW_HEIGHT)
			){
			return SCREEN_CULLED;
		}
		else{
			return NO_CULLED;
		}
	}
	/* *********************************************
	* 光栅化（顶点坐标已经转换到屏幕坐标）
	* *********************************************/
	// 插值
	template <typename VertexStruct>
	void RenderLine(VertexStruct v1, VertexStruct v2) {
		SimpleBrush::DrawLine_floatPos(
			v1.pos.ToVec2(), v1.color,
			v2.pos.ToVec2(), v2.color,
			m_imgBuffer_back);
	}

	template <typename VertexStruct>
	void RenderTriangle_wireframe(VertexStruct v1, VertexStruct v2, VertexStruct v3){
		RenderLine(v1, v2);
		RenderLine(v2, v3);
		RenderLine(v3, v1);
	}
	template <typename VertexStruct>
	void RenderTriangle_fill(VertexStruct v1, VertexStruct v2, VertexStruct v3){
		SimpleBrush::DrawTriangle(
			v1.pos.ToVec2(), v1.color, 
			v2.pos.ToVec2(), v2.color,
			v3.pos.ToVec2(), v3.color,
			m_imgBuffer_back);
	}
	
private:
	// 单例
	static RenderManager* m_instance;
	// 后台缓存
	ImgBuffer<ShortColor4>* m_imgBuffer_back;
	ImgBuffer<ShortColor4>* m_imgBuffer_front;
	// 深度缓存
	ImgBuffer<DepthBufferPixel>* m_imgBuffer_depth;
	// 贴图缓存
	ImgBuffer<NormColor4>* m_cur_texture;
};

#endif