#include "pch.h"
#include "Bomb.h"
#include "Engine.h"

#include "Collider.h"
#include "AssetMgr.h"
#include "TimeMgr.h"

void Bomb::Init()
{
	SetName(L"Bomb");

	SetScale(Vec2(120.f, 120.f));
	m_Tex = AssetMgr::GetInst()->LoadTexture(L"bomb", L"Texture\\BombDrone.bmp");

	m_Collider = AddComponent(new Collider);
	m_Collider->SetScale(Vec2(80.f, 80.f)); // 테스트 용
	// m_Collider->SetScale(GetScale()); -> 텍스쳐 넣으면 이거 쓰기
    m_speed = 600.f;


}

void Bomb::Tick()
{
    if (nullptr == m_Target)
    {
        // 디버깅용
        OutputDebugString(L"m_Target is NULL\n");
        return;
    }
    
    Vec2 pPos = m_Target->GetPos(); // 플레이어 위치
    Vec2 bPos = this->GetPos();     // 폭탄 위치

    Vec2 Dir = pPos - bPos;

    if (Dir.Length() != 0.f)
    {
        Dir.Normalize();
        bPos += Dir* m_speed * DT;
        SetPos(bPos);
    }
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
            RGB(255, 255, 255)); // 여백 투명
    }

    Actor::Render(_dc); // 디버그용 박스
}

void Bomb::BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
    if (_OtherCollider->GetName() == L"GuardBox" && _OtherCollider->IsEnable())
    {
        Destroy();
    }
}

void Bomb::Overlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
    if (_OtherCollider->GetName() == L"GuardBox" && _OtherCollider->IsEnable())
    {
        Destroy();
    }
}

void Bomb::EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
}
