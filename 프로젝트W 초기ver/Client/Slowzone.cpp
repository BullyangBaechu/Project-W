#include "pch.h"
#include "Slowzone.h"

#include "Engine.h"
#include "RigidBody.h"

#include "Actor.h"
#include "Collider.h"
#include "TimeMgr.h"
#include "Camera.h"
#include "Player.h"
#include "AssetMgr.h"
#include "Ground.h"


Slowzone::Slowzone()
	: m_Collider(nullptr)
{
	SetName(L"Slowzone");
	m_Tex = AssetMgr::GetInst()->LoadTexture(L"Slowzone", L"Texture\\SlowZone.bmp");

	m_Collider = AddComponent(new Collider);
	m_Collider->SetScale(Vec2(200.f, 50.f));
    m_Collider->SetOffset(Vec2(0.f, 0.f));
	m_Collider->SetEnable(true);

    //m_RigidBody = AddComponent<RigidBody>(new RigidBody);

}

Slowzone::~Slowzone()
{
}

void Slowzone::Init()
{
	m_SlowRatio = 0.03f;
}


void Slowzone::Tick()
{
	/*
	if (m_RigidBody == nullptr)
		return;

	m_RigidBody->SetGround(false);
	Vec2 SlowzonePos = GetPos();


	float HalfHeight = m_Collider->GetScale().y / 2.f;


	// 벽돌이 공중에 있다면
	if (SlowzonePos.y + HalfHeight < GROUND_Y - 48.f)
	{
		// 카메라 따라 우측 이동하는 보정
		float CamSpeed = Camera::GetInst()->GetCamSpeed();
		SlowzonePos.x += CamSpeed * DT; // 초당 300 속도 
		SetPos(SlowzonePos);
		m_RigidBody->SetGround(false);
	}
	// 벽돌이 바닥에 닿게 되면
	else
	{
		Vec2 Pos = GetPos();
		Pos.y = GROUND_Y - HalfHeight - 48.f;
		SetPos(Pos);

		m_RigidBody->SetGround(true);
	}
	*/
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

void Slowzone::BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
}

void Slowzone::EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
}

