#include "pch.h"
#include "Bomb.h"
#include "Engine.h"

#include "Collider.h"
#include "AssetMgr.h"

void Bomb::Init()
{
	SetName(L"Bomb");

	SetScale(Vec2(120.f, 120.f));
	m_Tex = AssetMgr::GetInst()->LoadTexture(L"bomb", L"Texture\\bomb_cleaned.bmp");

	m_Collider = AddComponent(new Collider);
	m_Collider->SetScale(Vec2(100.f, 100.f)); // 테스트 용
	// m_Collider->SetScale(GetScale()); -> 텍스쳐 넣으면 이거 쓰기
}

void Bomb::Tick()
{
}


void Bomb::Render(HDC _dc)
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
            RGB(255, 0, 255)); // 마젠타 투명
    }

    Actor::Render(_dc); // 디버그용 박스
}
