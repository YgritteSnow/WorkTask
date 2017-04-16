#ifndef __JTIME_H__
#define __JTIME_H__

#include <Windows.h>

typedef float TimeType;
/* *********************************************
* 计时器
* *********************************************/
class Ticker {
public:
	Ticker()
		: m_lastFrameInterval(-1)
		, m_lastSlip(0)
		, m_minFrameInterval(-1)
	{
		m_clockTime = GetTickCount();
	}
public:
	bool Tick();

	TimeType GetCurTime(){ return static_cast<TimeType>(m_clockTime); }
	TimeType GetFrameInterval(){ return m_lastFrameInterval; }
	TimeType GetFrameRate() { return 1 / m_lastFrameInterval; }

	void SetMinFrameInterval(TimeType t) { m_minFrameInterval = t; }
private:
	TimeType m_lastSlip;
	TimeType m_lastFrameInterval;
	DWORD m_clockTime;

	// 全局设置的值
	TimeType m_minFrameInterval;
};

/* *********************************************
* Time
* *********************************************/
class TimeManager {
private:
	TimeManager();
public:
	~TimeManager();
	static bool Init(TimeType maxFrameRate){
		m_instance = new TimeManager;
		if (m_instance == nullptr){
			return false;
		}
		m_instance->m_frame_ticker->SetMinFrameInterval(1 / maxFrameRate);
		m_instance->m_frame_ticker->SetMinFrameInterval(0.1f);
		return (m_instance != nullptr);
	}
	static void UnInit(){
		if (m_instance){
			delete m_instance;
			m_instance = nullptr;
		}
	}
	static TimeManager* GetInstance() { return m_instance; }
public:
	bool FrameTick() { return m_frame_ticker->Tick(); };
	TimeType GetFrameInterval() { return m_frame_ticker->GetFrameInterval(); }
	TimeType GetFrameRate() { return m_frame_ticker->GetFrameRate(); }

	bool AnimateTick() { return m_ani_ticker->Tick(); }
	TimeType GetAniInterval() { return m_ani_ticker->GetFrameInterval(); }
private:
	static TimeManager* m_instance;
	Ticker* m_frame_ticker;
	Ticker* m_ani_ticker;
};

#endif