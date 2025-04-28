#include "pch.h"
#include "Engine.h"

#include "AssetMgr.h"
#include "PathMgr.h"
#include "TimeMgr.h"
#include "LevelMgr.h"
#include "UIMgr.h"
#include "RenderMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "TaskMgr.h"
#include "Camera.h"
#include "SpawnMgr.h"

Engine::Engine()
    : m_hInst(nullptr)
    , m_hMainWnd(nullptr)
   
{
}

Engine::~Engine()
{
  
}

// window proc 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int Engine::Init(HINSTANCE _hInst, UINT _Width, UINT _Height)
{
    // 인스턴스 핸들
    m_hInst = _hInst;

    // 윈도우 생성
    WNDCLASSEXW wcex = {};
    wcex.lpszClassName = L"Key";
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // 생성시킬 윈도우의 정보 등록
    RegisterClassExW(&wcex);

    // 등록시킨 정보를 바탕으로 윈도우를 생성시킴
    m_hMainWnd = CreateWindowW(L"Key", L"MyGame", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

    // 윈도우 생성 실패 시 핸들값이 nullptr 이다.
    if (!m_hMainWnd)
    {
        MessageBox(nullptr, L"윈도우 생성 실패", L"에러", MB_OK);
        return E_FAIL;
    }

    // 윈도우 크기 변경
    ChangeWindowResolution(_Width, _Height);


    // Manager 초기화
    PathMgr::GetInst()->Init();
    KeyMgr::GetInst()->Init();
    AssetMgr::GetInst()->Init();

    // Render Manager 초기화
    if (FAILED(RenderMgr::GetInst()->Init(m_Resolution)))
    {
        return E_FAIL;
    }

    LevelMgr::GetInst()->Init();
    TimeMgr::GetInst()->Init();

  
    
    return S_OK;
}

void Engine::Progress()
{ 
    // Manager
    TimeMgr::GetInst()->Tick();
    KeyMgr::GetInst()->Tick();
    //Camera::GetInst()->Tick();

    // Level, Collision
    UIMgr::GetInst()->Tick();
    LevelMgr::GetInst()->Progress();
    CollisionMgr::GetInst()->Tick();
   
   


    // Render
    RenderMgr::GetInst()->Progress();  

    // Task
    TaskMgr::GetInst()->Tick();

    Camera::GetInst()->Tick();
}

void Engine::ChangeWindowResolution(UINT _Width, UINT _Height)
{
    m_Resolution = Vec2(_Width, _Height);

    // 해상도에 맞는 윈도우 크기 계산
    RECT rt = { 0, 0, (int)_Width, (int)_Height };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!GetMenu(m_hMainWnd));
        
    // 윈도우 좌상단 위치 및 크기 조정
    SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

    // 윈도우가 화면에 보이도록 설정
    ShowWindow(m_hMainWnd, true);

    // 윈도우 정보 갱신
    UpdateWindow(m_hMainWnd);
}