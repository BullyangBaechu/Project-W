#include "pch.h"
#include "TimeMgr.h"

#include "Engine.h"
#include "PathMgr.h"

TimeMgr::TimeMgr()
	: m_Frequency{}
	, m_Prev{}
	, m_Current{}
	, m_DeltaTime(0.f)
	, m_Time(0.f)
	, m_LimitDT(1.f/ 60.f)
	, m_CallCount(0)
{
}

TimeMgr::~TimeMgr()
{
}

void TimeMgr::Init()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_Prev);
}

void TimeMgr::Tick()
{
	QueryPerformanceCounter(&m_Current);

	// 프레임 간격 시간
	m_DeltaTime = (float)(m_Current.QuadPart - m_Prev.QuadPart) / (float)m_Frequency.QuadPart;

	// 시간값 누적
	m_Time += m_DeltaTime;

	if (m_DeltaTime > m_LimitDT)
	{
		m_DeltaTime = m_LimitDT;
	}

	// 1초에 호출되는 횟수(FPS)
	++m_CallCount;
	if (1.f < m_Time)
	{
		m_Time -= 1.f;

		wchar_t szBuff[100] = {};
		swprintf_s(szBuff, 100, L"FSP : %d, DT : %f, %s", m_CallCount, m_DeltaTime, PathMgr::GetInst()->GetBinPath());
		SetWindowText(Engine::GetInst()->GetMainWnd(), szBuff);

		m_CallCount = 0;
	}
		 
	m_Prev = m_Current;
}
