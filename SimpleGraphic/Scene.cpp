#include <algorithm>
#include "JMath.h"
#include "Scene.h"
#include "RenderManager.h"
#include "Material.h"
#include "BitMap.h"
#include <tchar.h>


/* *********************************************
* Scene & SceneManager
* *********************************************/
SceneManager* SceneManager::m_instance = nullptr;

void Scene::Render(){
	for (auto it = m_vec_model.begin(); it != m_vec_model.end(); ++it){
		(*it)->BeforeRender();
		RenderManager::GetInstance()->RenderVertexIndice(
			(*it)->GetVertexBuffer(),
			(*it)->GetIndexBuffer(),
			(*it)->GetModelMat()
			);
	}
}

void Scene::Update(TimeType delta_time){
}

bool Scene::HandleMouseEvent(const MouseEventObject& mouseEvent) {
	return false;
}
bool Scene::HanldeKeyEvent(const KeyEventObject& keyEvent) {
	return false;
}
bool Scene::HandleMouseMoveEvent(const MouseEventObject& mouseEvent) {
	// (debug)
	//float para = 0.01;
	//if (mouseEvent.is_down) {
	//	(*m_vec_model.begin())->RotateXYZ(mouseEvent.x_move * para,
	//		mouseEvent.y_move * para, mouseEvent.x_move * 0);
	//}
	return false;
}

bool SceneManager::Init(){
	m_instance = new SceneManager;
	if (!m_instance){
		return false;
	}
	return true;
}

void SceneManager::UnInit(){
	if (m_instance){
		delete m_instance;
		m_instance = nullptr;
	}
}

void SceneManager::Render() {
	RenderManager::GetInstance()->Clear();
	for (auto it = m_vec_scenes.begin(); it != m_vec_scenes.end(); ++it){
		(*it)->Render();
		break;
	}
	RenderManager::GetInstance()->Present();

	BitMap::DrawText(_T("asdfasdf"), ShortColor4(100, 100, 100, 100), ScreenPos(0, 0, 0));
}

void SceneManager::Update(TimeType delta_time){
	for (auto it = m_vec_scenes.begin(); it != m_vec_scenes.end(); ++it){
		(*it)->Update(delta_time);
		break;
	}
}
