#include "pch.h"
#include "Brick.h"
#include "RigidBody.h"

#include "Actor.h"
#include "Collider.h"
#include "TimeMgr.h"
#include "Camera.h"
#include "Player.h"

void Brick::Init(int type)
{
	m_MaxHP = type;
	m_CurHP = type;

	IsHit = false;

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
		// 카메라 따라 우측 이동하는 보정
		float CamSpeed = Camera::GetInst()->GetCamSpeed();
		BrickPos.x += CamSpeed * DT; // 초당 300 속도 
		SetPos(BrickPos);
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

	IsHit = false;
}

void Brick::Render(HDC _dc)
{
	Actor::Render(_dc); // 부모의 디버그 렌더링 호출
}

// 벽돌 피격 판정 충돌 로직
void Brick::BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
	if (_OtherCollider->GetName() == L"AttackBox")
	{
		// 이미 피격 판정이라면 그냥 return
		if (IsHit)
			return;
		
		// AttackBox의 Collider 도 결국 player의 Attack 함수로 생성된 Collider 이므로 attacker를 player로 dynamiccast 해준다.
		Actor* attacker = _OtherCollider->GetOwner();
		Player* pPlayer = dynamic_cast<Player*>(attacker);
		if (pPlayer)
		{
			IsHit = true;
			int dmg = pPlayer->GetPlayerDmg();
			Hit(dmg);
		}
	}
	
}

// 필요하면 쓸 예정
void Brick::EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
}
