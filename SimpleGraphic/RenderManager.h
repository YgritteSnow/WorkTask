#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

class RenderManager{
public:
	// ��ʼ��/����
	static bool Init();
	static void Uninit();
	static bool IsInited();
	// ��ö�����
	static RenderManager* GetInstance();
	static RenderManager* _instance;

public:
	// չʾ��Ⱦ���
	void Render();
};

#endif