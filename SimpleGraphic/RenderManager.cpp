#include "RenderManager.h"
#include <assert.h>

RenderManager* RenderManager::_instance = nullptr;

/* *********************************************
* 构造/析构
* *********************************************/
bool RenderManager::Init(){
	if (RenderManager::_instance == nullptr){
		RenderManager::_instance = new RenderManager();
	}
	return (RenderManager::_instance != nullptr);
}

void RenderManager::Uninit(){
	if (RenderManager::_instance == nullptr){
		delete RenderManager::_instance;
	}
	return;
}

bool RenderManager::IsInited(){
	return RenderManager::_instance != nullptr;
}

RenderManager* RenderManager::GetInstance(){
	assert(RenderManager::_instance != nullptr);
	return RenderManager::_instance;
}

/* *********************************************
 * 功能相关
 * *********************************************/
void RenderManager::Render(){
}