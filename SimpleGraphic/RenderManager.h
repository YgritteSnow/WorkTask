#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include <Windows.h>
#include "JMath.h"
#include "SimpleBrush.h"

typedef float DepthBufferPixel;

class RenderManager{
public:
	// ��ʼ��/����
	static bool Init(ScreenCoord width, ScreenCoord height);
	static void UnInit();
	static bool IsInited();
	// ���ʵ��
	static RenderManager* GetInstance();

private:
	// ��������
	bool SetupBuffer(ScreenCoord width, ScreenCoord height);
	// �ͷŻ���
	void ReleaseBuffer();

public:
	// �������
	void Clear();
	// ��Ⱦ����
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
	// �ύ����
	void Present();

private:
	/* *********************************************
	* �Զ�����д���
	* *********************************************/
	template <typename VertexStruct>
	void ProcessVertex(VertexStruct* src_vertex, VertexStruct** p_dst_vertex, size_t vertex_count, DWORD* indice, size_t indice_count, JMath::Mat44 modelMat){
		*p_dst_vertex = new VertexStruct[vertex_count];
		memcpy(*p_dst_vertex, src_vertex, vertex_count * sizeof(VertexStruct));
		ProcessVertex_pos(src_vertex, *p_dst_vertex, vertex_count, modelMat);
		// todo ������ʵ�ٶ��˶���struct�Ѿ�����normal��������ˣ��Ͼ����Ѽ�����ֱ��д���˼ҵ�normal�������ˡ�����
		// �Ժ��Ż���ʱ����Ը�������������������ʵ��ڴ�ṹ����������
		ProcessVertex_normal(src_vertex, *p_dst_vertex, vertex_count, indice, indice_count, modelMat);
		ProcessVertex_color(src_vertex, *p_dst_vertex, vertex_count, modelMat);
	}
	// ת������λ��
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
	// ���㶥�㷨����
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
	// ���㶥����ɫ��Gauroud��ֵ��
	template <typename VertexStruct>
	void ProcessVertex_color(VertexStruct* src_vertex, VertexStruct* dst_vertex, size_t vertex_count, JMath::Mat44 modelMat){
		
	}

	// ��Ⱦ�߶�
	void RenderLine_screen(ScreenPos p1, ScreenPos p2, ImgBuffer<Color4>* buffer);
	// ��Ⱦ������
	void RenderTriangle_fill_screen(ScreenPos p1, ScreenPos p2, ScreenPos p3, ImgBuffer<Color4>* buffer);
	void RenderTriangle_wareframe_screen(ScreenPos p1, ScreenPos p2, ScreenPos p3, ImgBuffer<Color4>* buffer);
	
public:
	// ����
	static RenderManager* m_instance;
	// ��̨����
	ImgBuffer<Color4>* m_imgBuffer_back;
	ImgBuffer<Color4>* m_imgBuffer_front;
	// ��Ȼ���
	ImgBuffer<DepthBufferPixel>* m_imgBuffer_depth;

private:
	
};

#endif