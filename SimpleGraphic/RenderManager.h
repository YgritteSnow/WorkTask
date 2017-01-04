#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include <Windows.h>
#include "JMath.h"
#include "SimpleBrush.h"

typedef float DepthBufferPixel;

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

public:
	// 清除缓存
	void Clear();
	// 渲染缓存
	template <typename VertexStruct>
	void RenderVertexIndice(VertexStruct* vertex, size_t vertex_count, DWORD* indice, size_t indice_count, JMath::Mat44 modelMat){
		VertexStruct* dst_vertex = nullptr;
		ProcessVertex(vertex, &dst_vertex, vertex_count, indice, indice_count, modelMat);

		for (size_t idx = 0; idx < indice_count - 2; idx+=3){
			size_t indice_idx_0 = indice[idx];
			size_t indice_idx_1 = indice[idx + 1];
			size_t indice_idx_2 = indice[idx + 2];

			auto p1 = ScreenPos((dst_vertex + indice_idx_0)->pos._x, (dst_vertex + indice_idx_0)->pos._y);
			auto p2 = ScreenPos((dst_vertex + indice_idx_1)->pos._x, (dst_vertex + indice_idx_1)->pos._y);
			auto p3 = ScreenPos((dst_vertex + indice_idx_2)->pos._x, (dst_vertex + indice_idx_2)->pos._y);

			RenderTriangle_wareframe_screen(p1, p2, p3, m_imgBuffer_back);
			//RenderTriangle_fill_screen(p1, p2, p3, m_imgBuffer_back);
		}

		delete[] dst_vertex;
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
		ProcessVertex_pos(src_vertex, *p_dst_vertex, vertex_count, modelMat);
		// todo 这里其实假定了顶点struct已经有了normal这个属性了，毕竟都把计算结果直接写在人家的normal属性里了。。。
		// 以后优化的时候可以根据输入条件申请更合适的内存结构来储存数据
		ProcessVertex_normal(src_vertex, *p_dst_vertex, vertex_count, indice, indice_count, modelMat);
		ProcessVertex_color(src_vertex, *p_dst_vertex, vertex_count, modelMat);
	}
	// 转换顶点位置
	template <typename VertexStruct>
	void ProcessVertex_pos(VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, JMath::Mat44 modelMat){
		for (size_t idx = 0; idx != vertex_count; ++idx){
			HomoPos tmp = modelMat.PreMulVec((dst_vertex + idx)->pos.ToVec4Pos());

			auto aaa = CameraManager::GetInstance()->CurrentCamera()->GetViewProjMat();
			auto bbb = CameraManager::GetInstance()->CurrentCamera()->GetViewMat();
			auto ccc = CameraManager::GetInstance()->CurrentCamera()->GetProjMat();

			tmp = CameraManager::GetInstance()->CurrentCamera()->GetViewProjMat().PreMulVec(tmp);
			WorldPos screen_pos = CameraManager::GetInstance()->CurrentCamera()->GetViewPortPos(tmp.ToVec3());
			(dst_vertex + idx)->pos = screen_pos;
		}
	}
	// 计算顶点法向量
	template <typename VertexStruct>
	void ProcessVertex_normal(VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count, JMath::Mat44 modelMat){
		for (size_t vertex_idx = 0; vertex_idx != vertex_count; ++vertex_idx){
			dst_vertex->normal.Zero();
		}

		for (size_t indice_idx = 0; indice_idx < indeice_count - 2; indice_idx += 3){
			DWORD vertex_idx_0 = indice[indice_idx];
			DWORD vertex_idx_1 = indice[indice_idx+1];
			DWORD vertex_idx_2 = indice[indice_idx+2];

			auto src_v01 = src_vertex[vertex_idx_1].pos - src_vertex[vertex_idx_0].pos;
			auto src_v12 = src_vertex[vertex_idx_2].pos - src_vertex[vertex_idx_1].pos;
			auto src_v20 = src_vertex[vertex_idx_0].pos - src_vertex[vertex_idx_2].pos;

			dst_vertex[vertex_idx_0]->normal += src_v20.CrossProduct(src_v01);
			dst_vertex[vertex_idx_1]->normal += src_v01.CrossProduct(src_v12);
			dst_vertex[vertex_idx_2]->normal += src_v12.CrossProduct(src_v20);
		}

		for (size_t vertex_idx = 0; vertex_idx != vertex_count; ++vertex_idx){
			dst_vertex->normal.Normalise();
		}
	}
	// 计算顶点颜色（Gauroud插值）
	template <typename VertexStruct>
	void ProcessVertex_color(VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, JMath::Mat44 modelMat){
		
	}

	// 渲染线段
	void RenderLine_screen(ScreenPos p1, ScreenPos p2, ImgBuffer<Color4>* buffer);
	// 渲染三角形
	void RenderTriangle_fill_screen(ScreenPos p1, ScreenPos p2, ScreenPos p3, ImgBuffer<Color4>* buffer);
	void RenderTriangle_wareframe_screen(ScreenPos p1, ScreenPos p2, ScreenPos p3, ImgBuffer<Color4>* buffer);
	
public:
	// 单例
	static RenderManager* m_instance;
	// 后台缓存
	ImgBuffer<Color4>* m_imgBuffer_back;
	ImgBuffer<Color4>* m_imgBuffer_front;
	// 深度缓存
	ImgBuffer<DepthBufferPixel>* m_imgBuffer_depth;

private:
	
};

#endif