#ifndef __INPUTEVENT_H__
#define __INPUTEVENT_H__

#include <vector>

/* *********************************************
* 键盘鼠标输入事件
* *********************************************/
struct MouseEventObject {
	MouseEventObject(int xm, int ym, int xc, int yc, bool d)
		: x_move(xm)
		, y_move(ym)
		, x_current(xc)
		, y_current(yc)
		, is_down(d)
	{}
	int x_move;
	int y_move;
	int x_current;
	int y_current;
	bool is_down;
};
struct KeyEventObject {
	KeyEventObject(int kp, bool d)
		: key_param(kp)
		, is_down(d)
	{}
	int key_param;
	bool is_down;
};

class InputEventHandler;
class InputEventHandlerManager {
public:
	static bool Init();
	static void UnInit();
	static InputEventHandlerManager* GetInstance();

public:
	InputEventHandlerManager()
		: m_isMouseInited(false)
		, m_mousePos_x(0)
		, m_mousePos_y(0)
		, m_isMouseDown(false) 
	{}
	void RegisterHandler(InputEventHandler* handler);
	void UnRegisterHandler(InputEventHandler* handler);

public:
	void HandleMouseEvent(bool is_down);
	void HandleKeyEvent(int key_param, bool is_down);
	void HandleMouseMoveEvent(int x_current, int y_current);

private:
	static InputEventHandlerManager* m_instance;
	std::vector<InputEventHandler*> m_inputEventHandlers;
	
	bool m_isMouseInited;
	int m_mousePos_x;
	int m_mousePos_y;
	bool m_isMouseDown;
};

class InputEventHandler {
public:
	InputEventHandler() {
		InputEventHandlerManager::GetInstance()->RegisterHandler(this);
	}
	~InputEventHandler() {
		InputEventHandlerManager::GetInstance()->UnRegisterHandler(this);
	}
	virtual bool HandleMouseEvent(const MouseEventObject& mouseEvent) { return false; };
	virtual bool HanldeKeyEvent(const KeyEventObject& keyEvent) { return false; };
	virtual bool HandleMouseMoveEvent(const MouseEventObject& mouseEvent) { return false; };
};

#endif
