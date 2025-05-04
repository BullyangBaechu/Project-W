#include "pch.h"
#include "Slowzone.h"

#include "AssetMgr.h"
#include "Collider.h"
#include "Texture.h"


Slowzone::Slowzone()
	: m_Collider(nullptr)
{
	SetName(L"SlowZone");
	m_Tex = AssetMgr::GetInst()->LoadTexture(L"SlowZone", L"Texture\\SlowZone.bmp");

	m_Collider = AddComponent(new Collider);
	m_Collider->SetScale(Vec2(200.f, 50.f));
    m_Collider->SetOffset(Vec2(0.f, 0.f));

}

Slowzone::~Slowzone()
{
}

void Slowzone::Init()
{
	m_SlowRatio = 0.3f;
}


void Slowzone::Tick()
{
}

void Slowzone::Render(HDC _dc)
{

    if (m_Tex)
    {
        Vec2 renderPos = GetRenderPos();
        float w = m_Tex->GetWidth();
        float h = m_Tex->GetHeight();

        TransparentBlt(_dc,
            (int)(renderPos.x - w / 2.f),
            (int)(renderPos.y - h / 2.f),
            (int)w,
            (int)h,
            m_Tex->GetDC(),
            0, 0,
            (int)w,
            (int)h,
            RGB(255, 255, 255)); // 여백 투명
    }

	Actor::Render(_dc); // 부모의 디버그 렌더링 호출
}

