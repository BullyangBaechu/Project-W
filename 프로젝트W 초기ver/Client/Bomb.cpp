#include "pch.h"
#include "Bomb.h"
#include "Engine.h"

#include "Collider.h"
#include "AssetMgr.h"
#include "TimeMgr.h"
#include "EffectActor.h"
#include "LevelMgr.h"
#include "Level.h"

void Bomb::Init()
{
	SetName(L"Bomb");

	SetScale(Vec2(120.f, 120.f));
	m_Tex = AssetMgr::GetInst()->LoadTexture(L"bomb", L"Texture\\BombDrone.bmp");
    
	m_Collider = AddComponent(new Collider);
	m_Collider->SetScale(Vec2(80.f, 80.f)); // 테스트 용
    m_Collider->SetEnable(true);
	// m_Collider->SetScale(GetScale()); -> 텍스쳐 넣으면 이거 쓰기
    m_speed = 900.f;


    // 파괴 용
    m_DestructionTimer = 0.3f;
    m_bDestroyed = false;
    //m_DestoryTex = AssetMgr::GetInst()->LoadTexture(L"bomb", L"Texture\\explosion_effect_whitebg.bmp");


}

void Bomb::Tick()
{
    if (m_bDestroyed)
    {
        m_DestructionTimer -= DT;
        if (m_DestructionTimer <= 0.f)
        {
            Destroy();
        }
        return;
    }
    
    
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

void Bomb::StartDestroy()
{
    if (m_bDestroyed) 
        return;

    m_bDestroyed = true;
    m_DestructionTimer = 0.3f;

    EffectActor* pEffect = new EffectActor;


    pEffect->SetEffect(AssetMgr::GetInst()->FindTexture(L"explosion"), 0.3f, AssetMgr::GetInst()->FindSound(L"bombdestroy")); // 사운드 생략하려면 nullptr
    pEffect->SetPos(GetPos());
    Level* pLevel = LevelMgr::GetInst()->GetCurrentLevel();
    pLevel->AddObject(ACTOR_TYPE::EFFECT, pEffect);
    //LevelMgr::GetInst()->GetCurrentLevel()->AddObject(ACTOR_TYPE::EFFECT, pEffect);

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
        StartDestroy();
        Destroy();       
    }
}

void Bomb::Overlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
    if (_OtherCollider->GetName() == L"GuardBox" && _OtherCollider->IsEnable())
    {
        StartDestroy();
        Destroy();
    }
}

void Bomb::EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
}
