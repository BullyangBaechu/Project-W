#pragma once

class Engine
{
	SINGLE(Engine);
private:
	HINSTANCE	m_hInst;		// 프로세스 핸들
	HWND		m_hMainWnd;		// 메인 윈도우 핸들
	Vec2		m_Resolution;	// 윈도우 비트맵 해상도
	 
public:
	void ChangeWindowResolution(UINT _Width, UINT _Height);
	Vec2 GetResolution() { return m_Resolution; }

	HWND GetMainWnd() { return m_hMainWnd ; }
	HINSTANCE GetMainInst() { return m_hInst; }
	

public:
	int Init(HINSTANCE _hInst, UINT _Width, UINT _Height);
	void Progress();
};