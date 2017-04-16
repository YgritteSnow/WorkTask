#include "JTime.h"
#include "DebugGUI.h"

/* *********************************************
* Ticker
* *********************************************/

bool Ticker::Tick() {
	DWORD new_clockTime = GetTickCount();
	TimeType new_slip = static_cast<TimeType>(new_clockTime - m_clockTime) / 1000;
	m_clockTime = new_clockTime;

	m_lastSlip += new_slip;
	if (m_lastSlip > m_minFrameInterval) {
		m_lastFrameInterval = m_lastSlip;
		m_lastSlip = 0;
		return true;
	}
	else {
		return false;
	}
}

/* *********************************************
* Time
* *********************************************/
TimeManager* TimeManager::m_instance = nullptr;

TimeManager::TimeManager()
	: m_ani_ticker(nullptr)
	, m_frame_ticker(nullptr)
{
	m_ani_ticker = new Ticker();
	m_frame_ticker = new Ticker();
}

TimeManager::~TimeManager()
{
	delete m_ani_ticker;
	m_ani_ticker = nullptr;

	delete m_frame_ticker;
	m_frame_ticker = nullptr;
}