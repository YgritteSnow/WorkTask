#include <algorithm>
#include "JMath.h"
#include "Scene.h"
#include "RenderManager.h"


/* *********************************************
* Scene & SceneManager
* *********************************************/
SceneManager* SceneManager::m_instance = nullptr;

void Scene::Render(){
	RenderManager::GetInstance()->Clear();
	for (auto it = m_vec_model.begin(); it != m_vec_model.end(); ++it){
		RenderManager::GetInstance()->RenderVertexIndice(
			(*it)->GetVertexes(),
			(*it)->GetVertexCount(),
			(*it)->GetIndices(),
			(*it)->GetIndiceCount(),
			(*it)->GetModelMat()
			);
	}
	//RenderManager::GetInstance()->RenderDummy();
	RenderManager::GetInstance()->Present();
}

void Scene::Update(TimeType delta_time){
	for (auto it = m_vec_model.begin(); it != m_vec_model.end(); ++it){
		(*it)->RotateXYZ(0, 0, 0.01);
	}
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

void SceneManager::Render(){
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