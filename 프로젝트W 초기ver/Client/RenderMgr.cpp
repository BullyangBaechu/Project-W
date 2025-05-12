#include "pch.h"
#include "RenderMgr.h"

#include "Engine.h"
#include "LevelMgr.h"
#include "Level.h"

#include "AssetMgr.h"
#include "Texture.h"

#include "Camera.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

RenderMgr::RenderMgr()
    : m_DC(nullptr)
    , m_BBTex(nullptr)
    , m_Pen{}
    , m_Brush{}
    , m_bDebugRenderToggle(false)
{

}

RenderMgr::~RenderMgr()
{    
    ReleaseDC(Engine::GetInst()->GetMainWnd(), m_DC);

    for (UINT i = 0; i < (UINT)PEN_TYPE::END; ++i)
    {
        DeleteObject(m_Pen[i]);
    }

    for (UINT i = 0; i < (UINT)BRUSH_TYPE::END; ++i)
    {
        DeleteObject(m_Brush[i]);
    }
}

int RenderMgr::Init(Vec2 _Resoution)
{
    m_Resolution = _Resoution;

    // DC 생성
    if (FAILED(CreateBuffer()))
    {
        return E_FAIL;
    }

    // GDI 오브젝트 생성
    CreateGDIObject();

    return S_OK;
}

void RenderMgr::Progress()
{
    Level* pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
    if (nullptr == pCurLevel)
        return;

    // 디버그 렌더 토글
    if (KEY_TAP(KEY::F8))
    {
        m_bDebugRenderToggle = !m_bDebugRenderToggle;
    }

    // ======
    // 렌더링
    // ======
    // 1. 백버퍼 클리어    
    m_BBTex->Clear(BRUSH_TYPE::GRAY);
          
    // 2. 레벨에 물체들을 백버퍼에 그림    
    pCurLevel->Render(m_BBTex->GetDC());

    // 3. 카메라 후처리(PostProcess) 효과
    Camera::GetInst()->Render(m_BBTex->GetDC());

    // 4. 디버그 렌더링
    if (m_bDebugRenderToggle)
    {
        DebugRender();
    }

    // 5. 백버퍼에 Actor 들을 그리고, 백버퍼를 메인 윈도우 비트맵에 복사시킨다.
    BitBlt( m_DC, 0, 0
         , (int)m_Resolution.x, (int)m_Resolution.y
         , m_BBTex->GetDC()
         , 0, 0, SRCCOPY);
}

void RenderMgr::DebugRender()
{
    HBRUSH hHollowBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_BBTex->GetDC(), hHollowBrush);

    list<tDebugShape>::iterator iter = m_ShapeList.begin();

    for (; iter != m_ShapeList.end();)
    {
        SEL_PEN(m_BBTex->GetDC(), iter->Pen);
                 
        switch (iter->Shape)
        {
        case DEBUG_SHAPE::RECT:
            Rectangle(m_BBTex->GetDC()
                , int(iter->Center.x - iter->Scale.x / 2.f)
                , int(iter->Center.y - iter->Scale.y / 2.f)
                , int(iter->Center.x + iter->Scale.x / 2.f)
                , int(iter->Center.y + iter->Scale.y / 2.f));
            break;

        case DEBUG_SHAPE::CIRCLE:
            Ellipse(m_BBTex->GetDC()
                , int(iter->Center.x - iter->Scale.x / 2.f)
                , int(iter->Center.y - iter->Scale.y / 2.f)
                , int(iter->Center.x + iter->Scale.x / 2.f)
                , int(iter->Center.y + iter->Scale.y / 2.f));
            break;

        case DEBUG_SHAPE::LINE:
        {
            Vec2 vStart = iter->Center;
            Vec2 vEnd = iter->Scale;
            MoveToEx(m_BBTex->GetDC(), (int)vStart.x, (int)vStart.y, nullptr);
            LineTo(m_BBTex->GetDC(), (int)vEnd.x, (int)vEnd.y);
        }
            break;
        }        

        iter->Age += DT;
        if (iter->Life < iter->Age)
        {
            iter = m_ShapeList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    SelectObject(m_BBTex->GetDC(), hPrevBrush);
}

int RenderMgr::CreateBuffer()
{    
    m_DC = GetDC(Engine::GetInst()->GetMainWnd());

    if (nullptr == m_DC)
    {
        MessageBox(nullptr, L"DC 생성 실패", L"에러", MB_OK);
        return E_FAIL;
    }
     
    // 백버퍼 텍스쳐 생성(윈도우 해상도랑 동일한 사이즈로 생성)
    m_BBTex = AssetMgr::GetInst()->CreateTexture(L"BackBuffer", (UINT)m_Resolution.x, (UINT)m_Resolution.y);

    return S_OK;
}

void RenderMgr::CreateGDIObject()
{
    // Pen 생성
    m_Pen[(int)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    m_Pen[(int)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    m_Pen[(int)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    m_Pen[(int)PEN_TYPE::GRAY] = CreatePen(PS_SOLID, 1, RGB(127, 127, 127));

    // Brush 생성
    m_Brush[(int)BRUSH_TYPE::RED] = CreateSolidBrush(RGB(255, 0, 0));
    m_Brush[(int)BRUSH_TYPE::GREEN] = CreateSolidBrush(RGB(0, 255, 0));
    m_Brush[(int)BRUSH_TYPE::BLUE] = CreateSolidBrush(RGB(0, 0, 255));
    m_Brush[(int)BRUSH_TYPE::GRAY] = CreateSolidBrush(RGB(127, 127, 127));
}
