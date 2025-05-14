#include "pch.h"
#include "BackGround.h"
#include "Camera.h"
#include "Engine.h"
#include "AssetMgr.h"

BackGround::BackGround()
    : m_ParallaxFactor(0.8f)
    , m_Tex(nullptr)
{
    SetName(L"BackGround");
    m_Tex = AssetMgr::GetInst()->LoadTexture(L"BackGround", L"Texture\\BackGroundFactory.bmp");
    m_InitialPos = Camera::GetInst()->GetLookAt();
    SetPos(m_InitialPos);
    //SetScale(Vec2(1440.f, 900.f));  
}

BackGround::~BackGround()
{
}



void BackGround::Tick()
{
}

void BackGround::Render(HDC _dc)
{
    //OutputDebugString(L"[Background Render 호출됨]\n");
    if (m_Tex)
    {
        //OutputDebugString(L"[Background 텍스쳐 있음]\n");

        Vec2 camPos = Camera::GetInst()->GetLookAt();
        Vec2 renderPos = GetRenderPos();
        Vec2 res = Engine::GetInst()->GetResolution();


        //float w = m_Tex->GetWidth();
        //float h = m_Tex->GetHeight();

        float parallaxX = (camPos.x - 720.f) * (1.0f - m_ParallaxFactor);
        renderPos.x -= parallaxX; // 원근 적용

        
         // 1. 반복 개수 구함
        float w = m_Tex->GetWidth();
        float h = m_Tex->GetHeight();
        float halfWidth = res.x / 2.f;

        int start = (int)((camPos.x - halfWidth) / w) - 1;
        int end = (int)((camPos.x + halfWidth) / w) + 1;

        

        // 2. 반복해서 그림
        for (int i = start; i <= end; ++i)
        {
            float worldX = i * w + w / 2.f;
            Vec2 worldPos(worldX, renderPos.y);
            Vec2 finalPos = Camera::GetInst()->GetRenderPos(worldPos);

            TransparentBlt(_dc,
                (int)(finalPos.x - w / 2.f),
                (int)(finalPos.y - h / 2.f),
                (int)w,
                (int)h,
                m_Tex->GetDC(),
                0, 0,
                (int)w,
                (int)h,
                RGB(255, 0, 255));
        }
        


        


        //TransparentBlt(_dc,(int)(renderPos.x - w / 2.f),(int)(renderPos.y - h / 2.f),(int)w,(int)h,m_Tex->GetDC(),0, 0,(int)w,(int)h, RGB(255, 0, 255));
    }

    //Actor::Render(_dc);
}