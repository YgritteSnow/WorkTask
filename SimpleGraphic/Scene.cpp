#include <algorithm>
#include "JMath.h"
#include "Scene.h"
#include "RenderManager.h"
#include "Material.h"
#include "BitMap.h"


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
	for (auto it = m_vec_scenes.begin(); it != m_vec_scenes.end(); ++it){
		(*it)->Render();
		break;
	}
}

void SceneManager::Update(TimeType delta_time){
	for (auto it = m_vec_scenes.begin(); it != m_vec_scenes.end(); ++it){
		(*it)->Update(delta_time);
		break;
	}
}
