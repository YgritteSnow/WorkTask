#include <algorithm>
#include "JMath.h"
#include "Scene.h"
#include "RenderManager.h"

/* *********************************************
* Camera & CameraManager
* *********************************************/
CameraManager* CameraManager::m_instance = nullptr;

void Camera::SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect){
	m_viewMat.SetViewMat(lookat, cameraPos, upDirect);
}
void Camera::SetProjMat(float fov, float aspect, float nearPlane, float farPlane){
	m_projMat.SetProjMat(fov, aspect, nearPlane, farPlane);
}
bool CameraManager::Init(){
	m_instance = new CameraManager;
	if (!m_instance){
		return false;
	}
	if (!m_instance->InitCamera()){
		return false;
	}
	return true;
}
void CameraManager::UnInit(){
	return;
}

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
	RenderManager::GetInstance()->Present();
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