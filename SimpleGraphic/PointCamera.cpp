#include "PointCamera.h"

PointCamera::PointCamera()
	: m_downMousePos()
	, m_downViewMat()
	, m_isDown(false)
{}

PointCamera::~PointCamera()
{}

bool PointCamera::HandleMouseEvent(const MouseEventObject& mouseEvent) {
	if (mouseEvent.is_down) {
		// 按下时记录鼠标位置
		m_downMousePos = ScreenPos(mouseEvent.x_current, mouseEvent.y_current, 0);
		m_downViewMat = m_viewMat;
	}
	m_isDown = mouseEvent.is_down;
	return false;
}
bool PointCamera::HanldeKeyEvent(const KeyEventObject& keyEvent) {
	return false;
}
bool PointCamera::HandleMouseMoveEvent(const MouseEventObject& mouseEvent) {
	float para = -0.01;
	if (m_isDown) {
		m_viewMat = m_downViewMat.RotateXYZ((mouseEvent.x_current - m_downMousePos._x) * para,
			(mouseEvent.y_current - m_downMousePos._y) * para, mouseEvent.x_move * 0);
		CalInvViewMat();
	}
	return false;
}