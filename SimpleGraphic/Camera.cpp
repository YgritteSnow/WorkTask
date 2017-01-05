#include "Camera.h"

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
void Camera::Update(TimeType delta_time){}

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

void CameraManager::Update(TimeType t){
	m_curCamera->Update(t);
}