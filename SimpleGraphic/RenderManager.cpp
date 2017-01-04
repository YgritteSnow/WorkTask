#include "RenderManager.h"
#include "BitMapDisplay.h"
#include <random>
/* *********************************************
* 构造/析构
* *********************************************/
bool RenderManager::Init(ScreenCoord width, ScreenCoord height){
	if (RenderManager::m_instance == nullptr){
		RenderManager::m_instance = new RenderManager();
		if (RenderManager::m_instance == nullptr) {
			return false; 
		}

		if (!RenderManager::m_instance->SetupBuffer(width, height)) {
			delete RenderManager::m_instance;
			RenderManager::m_instance = nullptr;
			return false;
		}
	}
	return (RenderManager::m_instance != nullptr);
}

void RenderManager::UnInit(){
	if (RenderManager::m_instance != nullptr){
		RenderManager::m_instance->ReleaseBuffer();
		delete RenderManager::m_instance;
	}
	return;
}

bool RenderManager::IsInited(){
	return RenderManager::m_instance != nullptr;
}

RenderManager* RenderManager::GetInstance(){
	return RenderManager::m_instance;
}

/* *********************************************
* 建立和释放缓存
* *********************************************/
bool RenderManager::SetupBuffer(ScreenCoord width, ScreenCoord height) {
	m_imgBuffer_back = new ImgBuffer<Color4>(width, height);
	if (!m_imgBuffer_back) { return false; }
	m_imgBuffer_front = new ImgBuffer<Color4>(width, height);
	if (!m_imgBuffer_front) { return false; }
	m_imgBuffer_depth = new ImgBuffer<DepthBufferPixel>(width, height);
	if (!m_imgBuffer_depth) { return false; }
	return true;
}

void RenderManager::ReleaseBuffer() {
	if (m_imgBuffer_back) {
		delete m_imgBuffer_back;
		m_imgBuffer_back = nullptr;
	}
	if (m_imgBuffer_front) {
		delete m_imgBuffer_front;
		m_imgBuffer_front = nullptr;
	}
	if (m_imgBuffer_depth) {
		delete m_imgBuffer_depth;
		m_imgBuffer_depth = nullptr;
	}
}

/* *********************************************
 * 渲染相关
 * *********************************************/
void RenderManager::RenderDummy(){
	// 渲染屏幕上一些随机的点（debug）
	//int i = 500;
	//while (--i > 0) {
	//	SimpleBrush::DrawDot_coordPos(
	//		i, 
	//		i,
	//		Color4(255, 255, 255, 255), 
	//		m_imgBuffer_back);
	//}

	// 渲染屏幕上一条线（debug）
	//RenderLine_screen(ScreenPos(333.333f, 166.666f), ScreenPos(125, 125));

	// 渲染一个三角形（debug）
	//RenderTriangle_fill_screen(ScreenPos(333.333, 166.666), ScreenPos(125, 125), ScreenPos(300, 100));
}

void RenderManager::Clear() {
	m_imgBuffer_back->clear();
}

void RenderManager::Present() {
	auto tmp = m_imgBuffer_front;
	m_imgBuffer_front = m_imgBuffer_back;
	m_imgBuffer_back = tmp;
	BitMapDisplay::Display<Color4>(m_imgBuffer_front);
}

/* *********************************************
* 光栅
* *********************************************/
void RenderManager::RenderDot_screen(ScreenPos p1) {
	SimpleBrush::DrawDot_coordPos((ScreenCoord)(p1._x+0.5f), (ScreenCoord)(p1._y+0.5f), Color4(255,255,255,255), m_imgBuffer_back);
}
void RenderManager::RenderLine_screen(ScreenPos p1, ScreenPos p2) {
	SimpleBrush::DrawLine_floatPos(p1, Color4(255, 0, 0, 255), p2, Color4(0,255,0,255), m_imgBuffer_back);
}

void RenderManager::RenderTriangle_wareframe_screen(ScreenPos p1, ScreenPos p2, ScreenPos p3){
	// 三角形边框
	RenderLine_screen(p1, p2);
	RenderLine_screen(p2, p3);
	RenderLine_screen(p3, p1);
}
void RenderManager::RenderTriangle_fill_screen(ScreenPos p1, ScreenPos p2, ScreenPos p3) {
	SimpleBrush::DrawTriangle(p1, Color4(255, 0, 255, 255), p2, Color4(0, 255, 255, 255), p3, Color4(255, 255, 0, 255), m_imgBuffer_back);
}

/* *********************************************
* 
* *********************************************/
RenderManager* RenderManager::m_instance = nullptr;