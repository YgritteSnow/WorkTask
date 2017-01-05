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
	m_imgBuffer_back = new ImgBuffer<ShortColor4>(width, height);
	if (!m_imgBuffer_back) { return false; }
	m_imgBuffer_front = new ImgBuffer<ShortColor4>(width, height);
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
	SimpleBrush::DrawLine_floatPos(
		ScreenPos(0, 0), NormColor4(1,0,0,1),
		ScreenPos(300, 300), NormColor4(0, 1, 0, 1),
		m_imgBuffer_back);
}

void RenderManager::Clear() {
	m_imgBuffer_back->clear();
}

void RenderManager::Present() {
	auto tmp = m_imgBuffer_front;
	m_imgBuffer_front = m_imgBuffer_back;
	m_imgBuffer_back = tmp;
	BitMapDisplay::Display<ShortColor4>(m_imgBuffer_front);
}

/* *********************************************
* 
* *********************************************/
RenderManager* RenderManager::m_instance = nullptr;