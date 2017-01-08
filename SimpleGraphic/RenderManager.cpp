#include "RenderManager.h"
#include "BitMap.h"
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
	m_imgBuffer_back = new ImgBuffer<NormColor4>(width, height);
	if (!m_imgBuffer_back) { return false; }
	m_imgBuffer_front = new ImgBuffer<NormColor4>(width, height);
	if (!m_imgBuffer_front) { return false; }
	m_imgBuffer_depth = new ImgBuffer<DepthBufferPixel>(width, height);
	if (!m_imgBuffer_depth) { return false; }
	m_imgBuffer_alpha = new AlphaBuffer(width, height);
	if (!m_imgBuffer_alpha) { return false; }
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
	if (m_imgBuffer_alpha) {
		delete m_imgBuffer_alpha;
		m_imgBuffer_alpha = nullptr;
	}
}

/* *********************************************
 * 渲染相关
 * *********************************************/
void RenderManager::RenderDummy(){}

void RenderManager::Clear() {
	m_imgBuffer_back->clear();
	m_imgBuffer_depth->clear(DepthBufferPixel(1.f));
	m_imgBuffer_alpha->clear();
}

void RenderManager::Present() {
	OnRenderFinish();
	auto tmp = m_imgBuffer_front;
	m_imgBuffer_front = m_imgBuffer_back;
	m_imgBuffer_back = tmp;
	BitMap::Display(m_imgBuffer_front);
}

void RenderManager::OnRenderFinish() {
	m_imgBuffer_alpha->SortAndBlend();
	for (int x = 0; x < m_imgBuffer_back->width; ++x) {
		for (int y = 0; y < m_imgBuffer_back->height; ++y) {
			auto pBackPixel = m_imgBuffer_back->pPixelAt(x, y);
			auto pAlphaPixel = m_imgBuffer_alpha->pPixelAt(x, y);
			*pBackPixel = BlendColor(*pBackPixel, pAlphaPixel->m_blended_color);
		}
	}
}
/* *********************************************
* 
* *********************************************/
RenderManager* RenderManager::m_instance = nullptr;