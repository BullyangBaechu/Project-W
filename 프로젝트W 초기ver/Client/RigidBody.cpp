#include "pch.h"
#include "RigidBody.h"

#include "TimeMgr.h"
#include "Actor.h"
#include "Collider.h"


RigidBody::RigidBody()
	: Component(COMPONENT_TYPE::RIGIDBODY)
	, m_Mass(1.f)	
	, m_Friction(500.f)
	, m_Move(false)
	, m_RigidBodyMode(RIGIDBODY_MODE::BELTSCROLL)	
	, m_MaxGravitySpeed(1000.f)
	, m_GravityAccel(1000.f)
	, m_JumpSpeed(500.f)
	, m_MaxJumpStack(1)
	, m_CurJumpStack(0)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::Jump()
{
	// 점프 여러 번 할 수 있을지? 체크할 때 사용
	if (m_CurJumpStack >= m_MaxJumpStack)
		return;

	m_Ground = false;
	IsJump = true;
	//m_GravityVelocity += -Vec2(0.f, m_JumpSpeed);
	m_GravityVelocity    = -Vec2(0.f, m_JumpSpeed);

	++m_CurJumpStack;
}

void RigidBody::FinalTick()
{

	if (GetOwner() == nullptr)
		return;

	if(RIGIDBODY_MODE::BELTSCROLL == m_RigidBodyMode)
		BeltScroll();
	
	// 힘 리셋
	m_Force = Vec2(0.f, 0.f);

	
}


void RigidBody::BeltScroll()
{
	
	if (GetOwner() == nullptr)
	{
		OutputDebugString(L"[RigidBody::BeltScroll] GetOwner() is nullptr!\n");
		return;
	}
	// 가속도 구하기
	Vec2 vAccel = m_Force / m_Mass;

	// 가속도에 따른 속도에 증가
	m_Velocity += vAccel * DT;
	float CurSpeed = m_Velocity.Length();

	// 초기속도 체크
	// 정지상태에서 출발하는 상황이면, 초기속력을 바로 준다.
	if (!m_Move && m_Ground && CurSpeed > 0.f)
	{
		m_Velocity += Vec2(m_Velocity).Normalize() * m_InitSpeed;
		CurSpeed = m_Velocity.Length();
	}

	// 마찰력으로 인한 속력 감소
	// 땅위에 서있을때만 마찰력 적용, Air 상태일 경우도 소량 마찰력이 필요해보임
	// 힘이 주어지지는 않지만 속력이 0 보다 큰 경우
	if (m_Force.IsZero() && m_Velocity.Length() > 0.f)
	{
		Vec2 vFrictionDir = -m_Velocity;
		vFrictionDir.Normalize();

		if (m_Ground)
			vFrictionDir *= m_Friction * DT;
		else
			vFrictionDir *= m_Friction * 0.1f * DT;

		// 마찰로 인한 속도 감소량이 기존 속도를 반대로 넘어설 수 없다.
		if (m_Friction * DT > m_Velocity.Length())
			m_Velocity = Vec2(0.f, 0.f);
		else
			m_Velocity += vFrictionDir;
	}

	// 최대속력 체크
	if (m_MaxSpeed < CurSpeed)
	{
		CurSpeed = m_MaxSpeed;
		m_Velocity.Normalize() *= CurSpeed;
	}

	// 중력가속도 체크
	// 땅위에 서있으면, 중력을 적용하지 않음
	if (m_Ground)
	{		
		m_GravityVelocity = Vec2(0.f, 0.f);
	}
	// 공중에 있는 경우
	else
	{
		// IsJump를 넣은 이유 : 부재 시 점프를 하면 바로 한 프레임만에 중력가속도를 받아서 점프 초기 속도에 상관없이 올라가는 높이 제어가 안 됨
		if (IsJump)
			IsJump = false;
		else
		{
			m_GravityVelocity.y += m_GravityAccel * DT;

			// 중력으로 인해 증가한 속도가 최대 제한치를 넘어서면, 맥스값으로 세팅
			if(m_GravityVelocity.y > m_MaxGravitySpeed)
				m_GravityVelocity.y = m_MaxGravitySpeed;
		}
		
		
	}

	// 최종 속도 = 움직임 속도 + 중력으로 인해 발생한 속도
	Vec2 vFinalVelocity = m_Velocity + m_GravityVelocity;

	// 최종 속도에 따른 이동
	Vec2 vActorPos = GetOwner()->GetPos();
	vActorPos += vFinalVelocity * DT;
	GetOwner()->SetPos(vActorPos);

	// 이동 상태 체크
	if (0.f < vFinalVelocity.Length())
	{
		m_Move = true;
	}

	// 속력이 0 이고, 받은 힘도 없을 때
	else if (m_Force.Length() == 0.f)
	{
		m_Move = false;
	}

	// Velocity 디버그 렌더링 요청
	Vec2 vVelo = vFinalVelocity;
	vVelo.Normalize() *= vFinalVelocity.Length() * 1.0f;
	DrawDebugLine(GetOwner()->GetRenderPos(), GetOwner()->GetRenderPos() + vVelo, PEN_TYPE::BLUE, 0.f);
}

void RigidBody::CollisionResponse(Actor* pOwner, Actor* pOther)
{
	//Vec2 myPos = pOwner->GetPos();
	//Vec2 otherPos = pOther->GetPos();
	//Vec2 myScale = pOwner->GetScale();
	//Vec2 otherScale = pOther->GetScale();
	
	// 실제 위치는 Actor 기준으로 조정해야 함
	//Vec2 actorPos = pOwner->GetPos(); 

	// 충돌 판정은 Collider 기준으로
	Collider* myCol = pOwner->GetComponent<Collider>();
	Collider* otherCol = pOther->GetComponent<Collider>();

	Vec2 myScale = myCol->GetScale();
	Vec2 otherScale = otherCol->GetScale();

	Vec2 myPos = myCol->GetFinalPos();
	Vec2 otherPos = otherCol->GetFinalPos();

	Vec2 delta = otherPos - myPos;

	float halfW = (myScale.x + otherScale.x) / 2.f;
	float halfH = (myScale.y + otherScale.y) / 2.f;

	

	float overlapX = halfW - abs(delta.x);
	float overlapY = halfH - abs(delta.y);

	// 우선 충돌한 축이 더 작은 쪽을 기준으로 충돌 방향 판단
	if (overlapX < overlapY)
	{
		Vec2 correctedCenter;
		// ← →
		if (delta.x > 0.f)
		{
			// 내가 왼쪽에 있음 → 오른쪽에서 막힘
			correctedCenter.x = otherPos.x - halfW - 1;
			//myPos.x = otherPos.x - halfW - 1.f;
		}
		else
		{
			// 내가 오른쪽에 있음 → 왼쪽에서 막힘
			correctedCenter.x = otherPos.x + halfW;
			//myPos.x = otherPos.x + halfW + 1.f;
		}
		correctedCenter.y = myPos.y;
		// X축 이동 정지
		m_Velocity.x = 0.f;
		Vec2 newActorPos = correctedCenter - myCol->GetOffset();
		pOwner->SetPos(newActorPos);
	}
	/* 스펙 변경으로 인해 y축 충돌 여부 없음
	else
	{
		// ↑ ↓
		if (delta.y > 0.f)
		{
			// 내가 위쪽에 있음 → 아래쪽 물체 위에 착지
			myPos.y = otherPos.y - halfH - 1.f;

			// 바닥에 착지
			m_Velocity.y = 0.f;
			m_GravityVelocity = Vec2(0.f, 0.f);
			if(pOther->GetActorType() == ACTOR_TYPE::PLATFORM || pOther->GetActorType() == ACTOR_TYPE::BRICK)
				m_Ground = true;
		}
		else
		{
			// 내가 아래쪽에 있음 → 머리 박음
			myPos.y = otherPos.y + halfH + 1.f;

			// 위로 튀지 않게 속도 제거
			m_Velocity.y = 0.f;
			m_GravityVelocity = Vec2(0.f, 0.f);
		}
	}
	*/

	
	//pOwner->SetPos(myPos);
}