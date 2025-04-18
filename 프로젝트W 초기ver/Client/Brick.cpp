#include "pch.h"
#include "Brick.h"
#include "RigidBody.h"

#include "Actor.h"
#include "Collider.h"

void Brick::Init(int type)
{
	m_MaxHP = type;
	m_CurHP = type;

	SetScale(Vec2(128.f, 128.f * type/2));
	
	// 참고용 void SetName(const wstring & _Name) { m_Name = _Name; }

	SetName(L"Brick_" + to_wstring(type));

	m_Collider = AddComponent(new Collider);
	//m_Collider->SetScale(Vec2(100.f, 100.f)); // 테스트 용
	m_Collider->SetScale(GetScale()); //-> 텍스쳐 넣으면 이거 쓰기

	m_Collider->SetOffset(Vec2(0.f, 0.f));


	m_RigidBody = AddComponent<RigidBody>(new RigidBody);
}

void Brick::Hit(int dmg)
{
	m_CurHP -= dmg;
	if (m_CurHP == 0)
		Destroy();
}

void Brick::Tick()
{

	//OutputDebugString(L"[Brick] Tick 호출됨\n");

	if (m_RigidBody == nullptr)
		return;

	m_RigidBody->SetGround(false);
	Vec2 BrickPos = GetPos();
	float HalfHeight = m_Collider->GetScale().y / 2.f;

	// 벽돌이 공중에 있다면
	if (BrickPos.y + HalfHeight < GROUND_Y)
	{
		m_RigidBody->SetGround(false);
	}
	// 벽돌이 바닥에 닿게 되면
	else
	{
		Vec2 Pos = GetPos();
		Pos.y = GROUND_Y - HalfHeight;
		SetPos(Pos);

		m_RigidBody->SetGround(true);
	}
}

void Brick::Render(HDC _dc)
{
	Actor::Render(_dc); // 부모의 디버그 렌더링 호출
}
