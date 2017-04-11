#include "CameraManager.h"

#include "PointCamera.h"

/* *********************************************
* CameraManager
* *********************************************/
CameraManager* CameraManager::m_instance = nullptr;

bool CameraManager::Init() {
	m_instance = new CameraManager;
	if (!m_instance) {
		return false;
	}
	if (!m_instance->InitCamera()) {
		return false;
	}

	return true;
}
void CameraManager::UnInit() {
	return;
}

void CameraManager::Update(TimeType t) {
	m_curCamera->Update(t);
}

bool CameraManager::InitCamera() {
	if (!m_curCamera) {
		m_curCamera = new PointCamera();
	}
	if (!m_curCamera) {
		return false;
	}
	return true;
}
Camera* CameraManager::CurrentCamera() {
	if (!m_instance) {
		return nullptr;
	}
	return m_instance->m_curCamera;
}
