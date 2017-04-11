#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Windows.h>
#include "JMath.h"
#include "JTime.h"
#include "InputEvent.h"

/* *********************************************
* Camera & CameraManager
* *********************************************/
extern UINT WINDOW_WIDTH;
extern UINT WINDOW_HEIGHT;

class Camera : public InputEventHandler {
public:
	Camera() :m_viewMat(), m_projMat() { CalInvViewMat(); }
	void SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect);
	void SetProjMat(float fov, float aspect, float nearPlane, float farPlane);
	JMath::Mat44 GetViewMat() const { return m_invViewMat; };
	JMath::Mat44 GetProjMat() const { return m_projMat; };
	JMath::Mat44 GetViewProjMat() const { return m_invViewMat.PostMulMat(m_projMat); };

	WorldPos TransToScreenPos(WorldPos pos) const { return WorldPos((pos._x + 0.5f) * WINDOW_WIDTH, (pos._y + 0.5f) * WINDOW_HEIGHT, pos._z); }

	WorldPos GetCameraPos() const { return m_viewMat.GetTranslate(); }

	void CalInvViewMat();

public:
	void Update(TimeType delta_time);

protected:
	// �������
	JMath::Mat44 m_viewMat;
	JMath::Mat44 m_invViewMat;
	// ͶӰ����
	JMath::Mat44 m_projMat;
	// �ӿھ���
	ScreenCoord m_screen_width, m_screen_height;
};


#endif