#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Windows.h>
#include "JMath.h"
#include "JTime.h"

/* *********************************************
* Camera & CameraManager
* *********************************************/
extern UINT WINDOW_WIDTH;
extern UINT WINDOW_HEIGHT;

class Camera{
public:
	Camera() :m_viewMat(), m_projMat(){}
	void DummyData(){
		this->SetViewMat(WorldPos(0, 0, 10), WorldPos(0, 0, 0), WorldPos(0, 1, 0));
		this->SetProjMat(0.4f, 1.f, 1.f, 1000.f);
	}
	void SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect);
	void SetProjMat(float fov, float aspect, float nearPlane, float farPlane);
	JMath::Mat44 GetViewMat() const { return m_viewMat; };
	JMath::Mat44 GetProjMat() const { return m_projMat; };
	JMath::Mat44 GetViewProjMat() const { return m_viewMat.PreMulMat(m_projMat); };
	WorldPos GetViewPortPos(WorldPos pos) const { return WorldPos((pos._x + 0.5f) * WINDOW_WIDTH, (pos._y + 0.5f) * WINDOW_HEIGHT, pos._z); }

public:
	void Update(TimeType delta_time);

private:
	// �������
	JMath::Mat44 m_viewMat;
	// ͶӰ����
	JMath::Mat44 m_projMat;
	// �ӿھ���
	ScreenCoord m_screen_width, m_screen_height;
};

class CameraManager{
private:
	CameraManager() :m_curCamera(nullptr){}
public:
	static bool Init();
	static void UnInit();
	static CameraManager* GetInstance(){ return m_instance; }

	void Update(TimeType t);

public:
	bool InitCamera(){
		if (!m_curCamera){
			m_curCamera = new Camera();
		}
		if (!m_curCamera){
			return false;
		}
		m_curCamera->DummyData();
		return true;
	}
	Camera* CurrentCamera(){
		if (!m_instance){
			return nullptr;
		}
		return m_instance->m_curCamera;
	}
public:
	static CameraManager* m_instance;
	Camera* m_curCamera;
};

#endif