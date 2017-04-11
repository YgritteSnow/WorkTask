#include "JTime.h"
#include "DebugGUI.h"

/* *********************************************
* Time
* *********************************************/
TimeManager* TimeManager::m_instance = nullptr;

bool TimeManager::Tick() {
	DWORD new_clockTime = GetTickCount();
	TimeType new_slip = static_cast<TimeType>(new_clockTime - m_clockTime) / 1000;
	m_clockTime = new_clockTime;

	m_lastSlip += new_slip;
	if (m_lastSlip > m_minFrameInterval) {
		m_lastFrameInterval = m_lastSlip;
		m_lastSlip = 0;
		DebugManager::GetInstance()->SetFPS(1.0 / m_lastFrameInterval);
		return true;
	}
	else {
		return false;
	}
}