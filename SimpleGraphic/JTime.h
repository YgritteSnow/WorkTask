#ifndef __JTIME_H__
#define __JTIME_H__

#include <Windows.h>

/* *********************************************
* Time
* *********************************************/
typedef float TimeType;

class TimeManager {
public:
	TimeManager()
		:m_lastFrameInterval(-1)
		, m_lastSlip(0)
		, m_minFrameInterval(-1){
		m_clockTime = GetTickCount();
	}

	static bool Init(TimeType maxFrameRate){
		m_instance = new TimeManager;
		if (m_instance == nullptr){
			return false;
		}
		m_instance->m_minFrameInterval = 1 / maxFrameRate;
		return (m_instance != nullptr);
	}
	static void UnInit(){
		if (m_instance){
			delete m_instance;
			m_instance = nullptr;
		}
	}
	static TimeManager* GetInstance(){ return m_instance; }
public:
	bool Tick(){
		DWORD new_clockTime = GetTickCount();
		TimeType new_slip = static_cast<TimeType>(new_clockTime - m_clockTime) / 1000;
		m_clockTime = new_clockTime;

		m_lastSlip += new_slip;
		if (m_lastSlip > m_minFrameInterval){
			m_lastFrameInterval = m_lastSlip;
			m_lastSlip = 0;
			return true;
		}
		else{
			m_lastSlip += m_minFrameInterval;
			return false;
		}
	}
	TimeType GetCurTime(){ return static_cast<TimeType>(m_clockTime); }
	TimeType GetFrameInterval(){ return m_lastFrameInterval; }
	TimeType GetFrameRate(){ return 1 / m_lastFrameInterval; }
private:
	static TimeManager* m_instance;

	TimeType m_lastSlip;
	TimeType m_lastFrameInterval;
	// 全局设置的值
	TimeType m_minFrameInterval;
	DWORD m_clockTime;
};

#endif