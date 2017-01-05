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
void Camera::Update(TimeType delta_time){
	//static TimeType tt = 0;
	//this->SetViewMat(WorldPos(tt*0.01, 0, 3), WorldPos(0, 0, 0), WorldPos(0, 1, 0));
	//tt += delta_time;
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

void CameraManager::Update(TimeType t){
	m_curCamera->Update(t);
}