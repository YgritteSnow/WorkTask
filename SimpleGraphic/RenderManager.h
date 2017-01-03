#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include "JMath.h"
#include "SimpleBrush.h"

typedef float DepthBufferPixel;

class RenderManager{
public:
	// 初始化/析构
	static bool Init(ScreenCoord width, ScreenCoord height);
	static void Uninit();
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
	void Render();
	// 提交缓存
	void Present();
private:
	// 渲染线段
	void RenderLine(ScreenPos p1, ScreenPos p2, ImgBuffer<Color4>* buffer);
	// 渲染三角形内部
	void RenderTriangle(ScreenPos p1, ScreenPos p2, ScreenPos p3, ImgBuffer<Color4>* buffer);
	
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