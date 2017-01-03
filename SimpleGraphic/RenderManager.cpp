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

void RenderManager::Uninit(){
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
void RenderManager::Render(){
	// for debug
	int i = 100;
	while (i-- > 0) {
		float x = ((float)rand() / RAND_MAX + 1) / 2;
		float y = ((float)rand() / RAND_MAX + 1) / 2;
		SimpleBrush::DrawDot_screen(
			//( (float)rand() / RAND_MAX + 1 )/ 2, 
			//((float)rand() / RAND_MAX + 1) / 2,
			x,
			y,
			Color4(255, 0, 255, 0), 
			m_imgBuffer_back);
	}
	//RenderLine(ScreenPos(0, 0), ScreenPos(100, 100), m_imgBuffer_back);
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

void RenderManager::RenderLine(ScreenPos p1, ScreenPos p2, ImgBuffer<Color4>* buffer) {
	SimpleBrush::DrawLine_screen(p1, p2, buffer);
}

void RenderManager::RenderTriangle(ScreenPos p1, ScreenPos p2, ScreenPos p3, ImgBuffer<Color4>* buffer) {

}

/* *********************************************
* 
* *********************************************/
RenderManager* RenderManager::m_instance = nullptr;