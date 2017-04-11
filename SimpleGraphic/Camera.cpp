#include "Camera.h"

/************************************************************************/
/* Camera                                                               */
/************************************************************************/
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