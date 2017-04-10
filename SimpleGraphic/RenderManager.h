#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include <Windows.h>

#include "JMath.h"
#include "Rasterizer.h"
#include "Light.h"
#include "TexBuffer.h"
#include "ABuffer.h"
#include "VertexUtilities.h"
#include "VertexBuffer.h"
#include "ShaderManager.h"
#include "LightingShader.h"
#include "DebugGUI.h"

extern UINT WINDOW_WIDTH;
extern UINT WINDOW_HEIGHT;


// 
class RenderManager{
public:
	RenderManager() :m_renderState(StateMaskNull){}

	// 初始化/析构
	static bool Init(ScreenCoord width, ScreenCoord height);
	static void UnInit();
	static bool IsInited();
	// 获得实例
	static RenderManager* GetInstance();

public:
	// 设置渲染状态
	void SetRenderState(StateMaskType state, StateMaskType value){ 
		m_renderState = m_renderState & (StateMaskAll ^ state);
		m_renderState = m_renderState | value;
	}
	bool CheckCurState(StateMaskType state, StateMaskType value){ 
		return CheckState(m_renderState, state, value);
	}

private:
	StateMaskType m_renderState;
private:
	// 建立缓存
	bool SetupBuffer(ScreenCoord width, ScreenCoord height);
	// 释放缓存
	void ReleaseBuffer();

public:
	// 清除缓存
	void Clear();
	// 渲染缓存
	void RenderVertexIndice(VertexBuffer* vb, IndexBuffer* ib, JMath::Mat44 modelMat) {
		// 顶点操作
		// 这一步要将顶点变换到屏幕空间
		auto vsShader = ShaderManager::GetInstance()->GetVertexShader();
		auto vb_temp = new VertexBuffer(vsShader->GetOutId(), vb->m_length);
		ShaderManager::ProcessVertex(vb, vb_temp);
		DebugManager::GetInstance()->SetVertexCount(vb->m_length);
		// 三角形操作
		// 这一步根据顶点位置和绕序，进行剔除
		MaskType* cull_mask = new MaskType[static_cast<int>(ib->size()/TRIANGLE_COUNT)];
		ProcessTriangle(vb_temp, ib, cull_mask);

		const auto& indice_count = ib->size();
		const auto& vb_vid = vb_temp->GetId();

		unsigned int tmp_triangle_count = 0;
		for (size_t idx = 0; idx < indice_count - 2; idx+=3){
			if (IS_CULLED(cull_mask[idx / 3])){
				continue;
			}
			tmp_triangle_count += 1;

			auto indice_idx_0 = (*ib)[idx];
			auto indice_idx_1 = (*ib)[idx + 1];
			auto indice_idx_2 = (*ib)[idx + 2];

			const auto& v0 = vb_temp->GetVertex(indice_idx_0);
			const auto& v1 = vb_temp->GetVertex(indice_idx_1);
			const auto& v2 = vb_temp->GetVertex(indice_idx_2);

			const auto& a1 = vb_temp->GetVertex(0);
			const auto& a2 = vb_temp->GetVertex(1);
			const auto& a3 = vb_temp->GetVertex(2);
			const auto& a4 = vb_temp->GetVertex(3);

			if (CheckCurState(StateMask_DrawMode, StateMaskValue_Wareframe)){
				RenderTriangle_wireframe(vb_vid, v0, v1, v2);
			}
			else if (CheckCurState(StateMask_DrawMode, StateMaskValue_Fill)){
				RenderTriangle_fill(vb_vid, v0, v1, v2);
			}
		}
		DebugManager::GetInstance()->SetValidTriangleCount(tmp_triangle_count);
		DebugManager::GetInstance()->SetTriangleCount(indice_count/3);

		delete[] cull_mask;
	}

	void RenderDummy();
	// 提交缓存
	void OnBeforePresent();
	void Present();

private:

	/* *********************************************
	* 对三角形进行处理
	* *********************************************/
	void ProcessTriangle(const VertexBuffer* pVbuffer, const IndexBuffer* ib, MaskType* cullMask) {
		for (int i = 0; i <= ib->size() - TRIANGLE_COUNT; i += TRIANGLE_COUNT) {
			auto pIndex = &((*ib)[i]);
			auto pMask = cullMask + i / TRIANGLE_COUNT;
			memset(pMask, NO_CULLED, sizeof(MaskType));

			DWORD indice_idx_0 = pIndex[0];
			DWORD indice_idx_1 = pIndex[1];
			DWORD indice_idx_2 = pIndex[2];

			const auto& v0 = pVbuffer->GetVertex(indice_idx_0);
			const auto& v1 = pVbuffer->GetVertex(indice_idx_1);
			const auto& v2 = pVbuffer->GetVertex(indice_idx_2);

			STRIDE_TYPE pos_off = StructReflectManager::GetOffset<POSITION>(pVbuffer->GetId(), 0);
			const auto& v0pos = *static_cast<WorldPos*>(static_cast<void*>((v0 + pos_off)));
			const auto& v1pos = *static_cast<WorldPos*>(static_cast<void*>((v1 + pos_off)));
			const auto& v2pos = *static_cast<WorldPos*>(static_cast<void*>((v2 + pos_off)));

			(*pMask) &= ProcessTriangle_backCull(v0pos, v1pos, v2pos, this->m_renderState);
			(*pMask) &= ProcessTriangle_screenCull(v0pos, v1pos, v2pos);
		}
	}
	/* *********************************************
	* 光栅化（顶点坐标已经转换到屏幕坐标）
	* *********************************************/
	// 插值
	void RenderLine(STRUCT_ID vid, byte* v1, byte* v2) {
		if (CheckCurState(StateMask_Alpha, StateMaskValue_NoAlpha)) {
			VertexBrush::DrawLine(vid, v1, v2, m_imgBuffer_back);
		}
		else if (CheckCurState(StateMask_Alpha, StateMaskValue_UseAlpha)) {
			VertexBrush::DrawLine(vid, v1, v2, m_imgBuffer_alpha);
		}
	}

	void RenderTriangle_wireframe(STRUCT_ID vid, byte* v1, byte* v2, byte* v3){
		RenderLine(vid, v1, v2);
		RenderLine(vid, v2, v3);
		RenderLine(vid, v3, v1);
	}

	void RenderTriangle_fill(STRUCT_ID vid, byte* v1, byte* v2, byte* v3) {
		if (CheckCurState(StateMask_Alpha, StateMaskValue_NoAlpha)) {
			VertexBrush::DrawTriangle(vid, v1, v2, v3, m_imgBuffer_back);
		}
		else if (CheckCurState(StateMask_Alpha, StateMaskValue_UseAlpha)) {
			VertexBrush::DrawTriangle(vid, v1, v2, v3, m_imgBuffer_alpha);
		}
	}

public:
	// 公共配置
	const auto& GetWidth() { return m_imgBuffer_back->GetWidth(); }
	const auto& GetHeight() { return m_imgBuffer_back->GetHeight(); }
	auto& GetDepthBuffer() { return m_imgBuffer_depth; }
	auto& GetBackBuffer() { return m_imgBuffer_back; }
	auto& GetAlphaBuffer() { return m_imgBuffer_alpha; }
	
private:
	// 单例
	static RenderManager* m_instance;
	// 后台缓存
	TexBuffer<NormColor4>* m_imgBuffer_back;
	TexBuffer<NormColor4>* m_imgBuffer_front;
	// 深度缓存
	TexBuffer<DepthBufferPixel>* m_imgBuffer_depth;
	// Alpha缓存
	AlphaBuffer* m_imgBuffer_alpha;
	// 贴图缓存
	TexBuffer<NormColor4>* m_cur_texture;
};

#endif