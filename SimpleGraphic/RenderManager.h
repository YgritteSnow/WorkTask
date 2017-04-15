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
	void SetRenderState(StateMaskType state, StateMaskType value);
	bool CheckCurState(StateMaskType state, StateMaskType value);

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
	void RenderVertexIndice(VertexBuffer* vb, IndexBuffer* ib, Matrix44 modelMat);
	// debug用
	void RenderDummy();
	// 提交缓存
	void OnBeforePresent();
	void Present();

private:

	/* *********************************************
	* 对三角形进行处理
	* *********************************************/
	void ProcessTriangle(const VertexBuffer* pVbuffer, const IndexBuffer* ib, MaskType* cullMask);
	/* *********************************************
	* 光栅化（顶点坐标已经转换到屏幕坐标）
	* *********************************************/
	void RenderLine(STRUCT_ID vid, byte* v1, byte* v2);
	void RenderTriangle_wireframe(STRUCT_ID vid, byte* v1, byte* v2, byte* v3);
	void RenderTriangle_fill(STRUCT_ID vid, byte* v1, byte* v2, byte* v3);

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