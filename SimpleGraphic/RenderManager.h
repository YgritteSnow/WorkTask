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

	// ��ʼ��/����
	static bool Init(ScreenCoord width, ScreenCoord height);
	static void UnInit();
	static bool IsInited();
	// ���ʵ��
	static RenderManager* GetInstance();

public:
	// ������Ⱦ״̬
	void SetRenderState(StateMaskType state, StateMaskType value);
	bool CheckCurState(StateMaskType state, StateMaskType value);

private:
	StateMaskType m_renderState;
private:
	// ��������
	bool SetupBuffer(ScreenCoord width, ScreenCoord height);
	// �ͷŻ���
	void ReleaseBuffer();

public:
	// �������
	void Clear();
	// ��Ⱦ����
	void RenderVertexIndice(VertexBuffer* vb, IndexBuffer* ib, Matrix44 modelMat);
	// debug��
	void RenderDummy();
	// �ύ����
	void OnBeforePresent();
	void Present();

private:

	/* *********************************************
	* �������ν��д���
	* *********************************************/
	void ProcessTriangle(const VertexBuffer* pVbuffer, const IndexBuffer* ib, MaskType* cullMask);
	/* *********************************************
	* ��դ�������������Ѿ�ת������Ļ���꣩
	* *********************************************/
	void RenderLine(STRUCT_ID vid, byte* v1, byte* v2);
	void RenderTriangle_wireframe(STRUCT_ID vid, byte* v1, byte* v2, byte* v3);
	void RenderTriangle_fill(STRUCT_ID vid, byte* v1, byte* v2, byte* v3);

public:
	// ��������
	const auto& GetWidth() { return m_imgBuffer_back->GetWidth(); }
	const auto& GetHeight() { return m_imgBuffer_back->GetHeight(); }
	auto& GetDepthBuffer() { return m_imgBuffer_depth; }
	auto& GetBackBuffer() { return m_imgBuffer_back; }
	auto& GetAlphaBuffer() { return m_imgBuffer_alpha; }
	
private:
	// ����
	static RenderManager* m_instance;
	// ��̨����
	TexBuffer<NormColor4>* m_imgBuffer_back;
	TexBuffer<NormColor4>* m_imgBuffer_front;
	// ��Ȼ���
	TexBuffer<DepthBufferPixel>* m_imgBuffer_depth;
	// Alpha����
	AlphaBuffer* m_imgBuffer_alpha;
	// ��ͼ����
	TexBuffer<NormColor4>* m_cur_texture;
};

#endif