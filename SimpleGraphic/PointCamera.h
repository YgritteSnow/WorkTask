#pragma once

#include "JMath.h"
#include "Camera.h"

class PointCamera : public Camera
{
public:
	PointCamera();
	~PointCamera();
public:
	virtual bool HandleMouseEvent(const MouseEventObject& mouseEvent) override;
	virtual bool HanldeKeyEvent(const KeyEventObject& keyEvent) override;
	virtual bool HandleMouseMoveEvent(const MouseEventObject& mouseEvent) override;

private:
	JMath::Mat44 m_downViewMat;
	ScreenPos m_downMousePos;
	bool m_isDown;
};