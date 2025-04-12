#include "pch.h"
#include "Missile.h"

#include "TimeMgr.h"
#include "Collider.h"

#include "Engine.h"

Missile::Missile()
	: m_Speed(500.f)
	, m_Collider(nullptr)
	, m_Time(0.f)
	, m_Angle(0.f)
{
	m_Collider = AddComponent(new Collider);
	m_Collider->SetScale(Vec2(18.f, 18.f));
}

Missile::~Missile()
{
}

void Missile::Tick()
{
	Move();

	ScreenCheck();
}

void Missile::Move()
{
	Vec2 vPos = GetPos();
		
	vPos += m_Velocity * DT;
	
	SetPos(vPos);
}

void Missile::ScreenCheck()
{
	Vec2 vPos = GetPos();

	// 화면밖으로 벗어난 순간부터 시간을 체크한다.
	Vec2 vResol = Engine::GetInst()->GetResolution();
	if (vPos.x < 0.f || vResol.x < vPos.x
		|| vPos.y < 0.f || vResol.y < vPos.y)
	{
		m_Time += DT;

		// 화면 밖으로 나간지 5초가 지났다면
		if (3.f <= m_Time)
		{
			// 스스로를 삭제시킨다.
			Destroy();
		}
	}
}


void Missile::Render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Ellipse(_dc
		, (int)(vPos.x - vScale.x / 2.f)
		, (int)(vPos.y - vScale.y / 2.f)
		, (int)(vPos.x + vScale.x / 2.f)
		, (int)(vPos.y + vScale.y / 2.f));

	// 디버그 렌더링
	Vec2 vAdd = m_Velocity;
	vAdd.Normalize() *= 50.f;
	DrawDebugLine(GetPos(), GetPos() + vAdd, PEN_TYPE::BLUE, 0.f);
}

void Missile::BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
	if (_OtherActor->GetName() == L"Monster")
	{
		Destroy();
	}
}
