#include <algorithm>
#include "InputEvent.h"

InputEventHandlerManager* InputEventHandlerManager::m_instance = nullptr;

bool InputEventHandlerManager::Init() {
	m_instance = new InputEventHandlerManager();
	if (m_instance == nullptr) {
		return false;
	}
	return true;
}

void InputEventHandlerManager::UnInit() {
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

InputEventHandlerManager* InputEventHandlerManager::GetInstance() {
	return m_instance;
}

void InputEventHandlerManager::RegisterHandler(InputEventHandler* handler) {
	m_inputEventHandlers.push_back(handler);
}

void InputEventHandlerManager::UnRegisterHandler(InputEventHandler* handler) {
	auto it = std::find(m_inputEventHandlers.begin(), m_inputEventHandlers.end(), handler);
	if (it == m_inputEventHandlers.end()) {
		return;
	}
	m_inputEventHandlers.erase(it);
}

void InputEventHandlerManager::HandleMouseEvent(bool is_down) {
	MouseEventObject m(0, 0, m_mousePos_x, m_mousePos_y, is_down);
	m_isMouseDown = is_down;
	for (auto it = m_inputEventHandlers.begin(); it != m_inputEventHandlers.end(); ++it) {
		(*it)->HandleMouseEvent(m);
	}
}

void InputEventHandlerManager::HandleKeyEvent(int key_param, bool is_down) {
	KeyEventObject m(key_param, is_down);
	for (auto it = m_inputEventHandlers.begin(); it != m_inputEventHandlers.end(); ++it) {
		(*it)->HanldeKeyEvent(m);
	}
}
void InputEventHandlerManager::HandleMouseMoveEvent(int x_current, int y_current) {
	if (!m_isMouseInited) {
		m_mousePos_x = x_current;
		m_mousePos_y = y_current;
		m_isMouseInited = true;
		return;
	}
	MouseEventObject m(x_current - m_mousePos_x, 
		y_current - m_mousePos_y,
		x_current,
		y_current,
		m_isMouseDown
		);
	m_mousePos_x = x_current;
	m_mousePos_y = y_current;
	for (auto it = m_inputEventHandlers.begin(); it != m_inputEventHandlers.end(); ++it) {
		(*it)->HandleMouseMoveEvent(m);
	}
}