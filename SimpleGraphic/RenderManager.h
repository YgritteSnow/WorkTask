#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

class RenderManager{
public:
	// 初始化/析构
	static bool Init();
	static void Uninit();
	static bool IsInited();
	// 获得对象单例
	static RenderManager* GetInstance();
	static RenderManager* _instance;

public:
	// 展示渲染结果
	void Render();
};

#endif