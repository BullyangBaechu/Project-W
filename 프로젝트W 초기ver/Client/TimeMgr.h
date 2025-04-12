#pragma once



class TimeMgr
{
	SINGLE(TimeMgr);
private:
	LARGE_INTEGER	m_Frequency;	// 초당 카운팅 횟수
	LARGE_INTEGER	m_Prev;			// 이전 프레임 카운팅
	LARGE_INTEGER	m_Current;		// 이번 프레임 카운팅

	float			m_DeltaTime;	// 1프레임 수행하는데 걸리는 시간
	float			m_Time;			// 프로그램을 실행한 시간
	float			m_LimitDT;

	UINT			m_CallCount;

public:
	void Init();
	void Tick();

public:
	float GetDeltaTime() { return m_DeltaTime; }
	float GetTime() { return m_Time; }
};

