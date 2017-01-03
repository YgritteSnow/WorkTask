#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include "JMath.h"
#include "SimpleBrush.h"

typedef float DepthBufferPixel;

class RenderManager{
public:
	// ��ʼ��/����
	static bool Init(ScreenCoord width, ScreenCoord height);
	static void Uninit();
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
	void Render();
	// �ύ����
	void Present();
private:
	// ��Ⱦ�߶�
	void RenderLine(ScreenPos p1, ScreenPos p2, ImgBuffer<Color4>* buffer);
	// ��Ⱦ�������ڲ�
	void RenderTriangle(ScreenPos p1, ScreenPos p2, ScreenPos p3, ImgBuffer<Color4>* buffer);
	
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