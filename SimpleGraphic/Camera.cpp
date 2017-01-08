#include "Camera.h"

/* *********************************************
* Camera & CameraManager
* *********************************************/
CameraManager* CameraManager::m_instance = nullptr;

void Camera::SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect){
	m_viewMat.SetViewMat(lookat, cameraPos, upDirect);
	CalInvViewMat();
}
void Camera::SetProjMat(float fov, float aspect, float nearPlane, float farPlane){
	m_projMat.SetProjMat(fov, aspect, nearPlane, farPlane);
}
void Camera::CalInvViewMat() {
	m_invViewMat = m_viewMat.Inverse();
}
void Camera::Update(TimeType delta_time) {}

bool Camera::HandleMouseEvent(const MouseEventObject& mouseEvent) {
	return false;
}
bool Camera::HanldeKeyEvent(const KeyEventObject& keyEvent) {
	return false;
}
bool Camera::HandleMouseMoveEvent(const MouseEventObject& mouseEvent) {
	float para = -0.001;
	if (mouseEvent.is_down) {
		m_viewMat = m_viewMat.RotateXYZ(mouseEvent.x_move * para,
			mouseEvent.y_move * para, mouseEvent.x_move * 0);
		CalInvViewMat();
	}
	return false;
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