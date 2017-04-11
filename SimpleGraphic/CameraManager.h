#pragma once

#include "Camera.h"

class CameraManager {
private:
	CameraManager() :m_curCamera(nullptr) {}
public:
	static bool Init();
	static void UnInit();
	static CameraManager* GetInstance() { return m_instance; }

	void Update(TimeType t);

public:
	bool InitCamera();
	Camera* CurrentCamera();
public:
	static CameraManager* m_instance;
	Camera* m_curCamera;
};