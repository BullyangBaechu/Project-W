#include "pch.h"
#include "KeyMgr.h"

#include "Engine.h"
#include "TimeMgr.h"

int g_KeyValue[(int)KEY::KEY_END] =
{
	'Q', 'W', 'E',  'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', 
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

	VK_LBUTTON, VK_RBUTTON, 
	VK_MENU, VK_CONTROL, VK_SPACE, VK_ESCAPE,
	VK_RETURN, VK_LSHIFT, 
	VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, 
	VK_F1, VK_F2, VK_F3, VK_F4,
	VK_F5, VK_F6, VK_F7, VK_F8,
};

KeyMgr::KeyMgr()
	: m_LockTime(0.f)
	, m_CurTime(0.f)
	, m_bLocked(false)
{
}

KeyMgr::~KeyMgr()
{
}

void KeyMgr::SetLock(float _LockFrame)
{
	m_LockTime = _LockFrame;
	m_CurTime = 0.f;
	m_bLocked = true;

	for (int i = 0; i < (int)KEY::KEY_END; ++i)
	{
		if (KEY_STATE::TAP == m_vecKey[i].State || KEY_STATE::PRESSED == m_vecKey[i].State)
			m_vecKey[i].State = KEY_STATE::PRESSED;
		else if (KEY_STATE::RELEASED == m_vecKey[i].State)
			m_vecKey[i].State = KEY_STATE::NONE;
	}
}

void KeyMgr::Init()
{
	for (int i = 0; i < (int)KEY::KEY_END; ++i)
	{
		m_vecKey.push_back(tKeyState{ KEY_STATE::NONE, false });
	}
}

void KeyMgr::Tick()
{
	// 실행중인 프로그램이 포커싱이 되어있다면
	// Lock 이 걸려있지 않아야한다.
	if (nullptr != GetFocus() && !m_bLocked)
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			// 해당 키가 눌린적이 있다면
			if (GetAsyncKeyState(g_KeyValue[i]))
			{
				// 해당 키가 눌렸고, 이전 프레임에도 눌렸었다.
				if (m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::PRESSED;
				}

				// 해당 키가 눌렸고, 이전에는 눌린적이 없다
				else
				{
					// 막 눌린 프레임
					m_vecKey[i].State = KEY_STATE::TAP;
				}

				m_vecKey[i].PrevPressed = true;
			}

			// 해당 키가 눌리지 않았다.
			else
			{
				// 해당키가 지금은 안눌려있고, 이전 프레임까지는 눌려있었다.
				if (m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::RELEASED;
				}

				// 이전에도 누르지 않았고, 지금도 눌려있지 않다.
				else
				{
					m_vecKey[i].State = KEY_STATE::NONE;
				}

				m_vecKey[i].PrevPressed = false;
			}
		}

		// 이전 프레임정보 유지
		m_PrevMousePos = m_MousePos;

		// 이번 프레임 마우스포지션 계산
		POINT ptMouse = {};
		GetCursorPos(&ptMouse);		
		ScreenToClient(Engine::GetInst()->GetMainWnd(), &ptMouse);		
		m_MousePos = ptMouse;

		// 마우스 진행 방향정보
		m_MouseDir = m_MousePos - m_PrevMousePos;
	}

	// 프로그램이 포커싱이 아닐 경우
	else
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			// 포커싱이 아니거나, Lock 상태일때 눌린 사항이, 다시 정상화됐을때 반영되지 않게
			// 지속적으로 실제 눌린 사항에대한 반응을 소모시키고 있는다.
			GetAsyncKeyState(g_KeyValue[i]);

			m_vecKey[i].PrevPressed = false;
			if (KEY_STATE::NONE == m_vecKey[i].State)			
				continue;

			if (KEY_STATE::TAP == m_vecKey[i].State || KEY_STATE::PRESSED == m_vecKey[i].State)
				m_vecKey[i].State = KEY_STATE::RELEASED;
			else if(KEY_STATE::RELEASED == m_vecKey[i].State)
				m_vecKey[i].State = KEY_STATE::NONE;
		}
	}

	m_CurTime += DT;
	if (m_LockTime <= m_CurTime)
	{
		m_bLocked = false;
	}
}