#include "pch.h"
#include "Monster.h"

#include "Engine.h"
#include "Collider.h"

#include "TimeMgr.h"

#include "AssetMgr.h"
#include "Texture.h"

#include "StateMachine.h"
#include "IdleMon.h"
#include "TraceMon.h"

Monster::Monster()
	: m_Texture(nullptr)
	, m_HP(5)
	, m_SM(nullptr)
	, m_DetectRange(200)
	, m_MoveSpeed(100.f)
{
	m_Texture = AssetMgr::GetInst()->LoadTexture(L"Sonic", L"Texture\\smokeparticle.png");
	SetScale(Vec2(m_Texture->GetWidth(), m_Texture->GetHeight()));

	m_Collider = AddComponent(new Collider);
	m_Collider->SetOffset(Vec2(0.f, 10.f));
	m_Collider->SetScale(Vec2(50.f, 75.f));

	m_SM = AddComponent(new StateMachine);
	m_SM->AddState(L"IdleMon", new IdleMon);
	m_SM->AddState(L"TraceMon", new TraceMon);
	m_SM->ChangeState(L"IdleMon");
}

Monster::~Monster()
{
	
}

void Monster::Tick()
{
	
}

void Monster::Render(HDC _dc)
{
	Vec2 vPos = GetRenderPos();

	int width = m_Texture->GetWidth();
	int height = m_Texture->GetHeight();

	//TransparentBlt(_dc
	//	, int(vPos.x - width / 2.f)
	//	, int(vPos.y - height / 2.f)
	//	, width, height
	//	, m_Texture->GetDC()
	//	, 0, 0, (int)width, (int)height
	//	, RGB(255, 0, 255));

	BLENDFUNCTION blend = {};
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;

	AlphaBlend(_dc
			, int(vPos.x - width / 2.f)
			, int(vPos.y - height / 2.f)
			, width, height
			, m_Texture->GetDC()
			, 0, 0, (int)width, (int)height
			, blend);
}

void Monster::BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
	if (_OtherActor->GetName() == L"Missile")
	{
		--m_HP;

		if (m_HP <= 0)
		{
			// 스스로를 삭제시킨다.
			//Destroy();
		}
	}
}