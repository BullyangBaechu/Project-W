#include "pch.h"
#include "RigidBody.h"

#include "TimeMgr.h"
#include "Actor.h"


RigidBody::RigidBody()
	: Component(COMPONENT_TYPE::RIGIDBODY)
	, m_Mass(1.f)	
	, m_Friction(500.f)
	, m_Move(false)
	, m_RigidBodyMode(RIGIDBODY_MODE::TOPVIEW)	
	, m_MaxGravitySpeed(1000.f)
	, m_GravityAccel(980.f)
	, m_JumpSpeed(100.f)
	, m_MaxJumpStack(3)
	, m_CurJumpStack(0)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::Jump()
{
	if (m_CurJumpStack >= m_MaxJumpStack)
		return;

	m_Ground = false;
	//m_GravityVelocity += -Vec2(0.f, m_JumpSpeed);
	m_GravityVelocity    = -Vec2(0.f, m_JumpSpeed);

	++m_CurJumpStack;
}

void RigidBody::FinalTick()
{
	if(RIGIDBODY_MODE::TOPVIEW == m_RigidBodyMode)
		TopView();
	else
		BeltScroll();

	// 힘 리셋
	m_Force = Vec2(0.f, 0.f);

	
}

void RigidBody::TopView()
{
	// 가속도 구하기
	Vec2 vAccel = m_Force / m_Mass;

	// 가속도에 따른 속도에 증가
	m_Velocity += vAccel * DT;
	float CurSpeed = m_Velocity.Length();

	// 초기속도 체크
	// 정지상태에서 출발하는 상황이면, 초기속력을 바로 준다.
	if (false == m_Move && 0.f < CurSpeed)
	{
		m_Velocity += Vec2(m_Velocity).Normalize() * m_InitSpeed;
		CurSpeed = m_Velocity.Length();
	}


	// 마찰력으로 인한 속력 감소
	// 힘이 주어지지는 않지만 속력이 0 보다 큰 경우
	if (m_Force.IsZero() && 0.f < m_Velocity.Length())
	{
		Vec2 vFrictionDir = -m_Velocity;
		vFrictionDir.Normalize();
		vFrictionDir *= m_Friction * DT;

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

	// 속도에 따른 이동
	Vec2 vActorPos = GetOwner()->GetPos();
	vActorPos += m_Velocity * DT;
	GetOwner()->SetPos(vActorPos);

	// 이동 상태 체크
	if (0.f < m_Velocity.Length())
	{
		m_Move = true;
	}

	// 속력이 0 이고, 받은 힘도 없을 때
	else if (m_Force.Length() == 0.f)
	{
		m_Move = false;
	}

	// Velocity 디버그 렌더링 요청
	Vec2 vVelo = m_Velocity;
	vVelo.Normalize() *= m_Velocity.Length() * 1.0f;
	DrawDebugLine(GetOwner()->GetRenderPos(), GetOwner()->GetRenderPos() + vVelo, PEN_TYPE::BLUE, 0.f);
}

void RigidBody::BeltScroll()
{
	// 가속도 구하기
	Vec2 vAccel = m_Force / m_Mass;

	// 가속도에 따른 속도에 증가
	m_Velocity += vAccel * DT;
	float CurSpeed = m_Velocity.Length();

	// 초기속도 체크
	// 정지상태에서 출발하는 상황이면, 초기속력을 바로 준다.
	if (false == m_Move && m_Ground && 0.f < CurSpeed)
	{
		m_Velocity += Vec2(m_Velocity).Normalize() * m_InitSpeed;
		CurSpeed = m_Velocity.Length();
	}

	// 마찰력으로 인한 속력 감소
	// 땅위에 서있을때만 마찰력 적용, Air 상태일 경우도 소량 마찰력이 필요해보임
	// 힘이 주어지지는 않지만 속력이 0 보다 큰 경우
	if (m_Force.IsZero() && 0.f < m_Velocity.Length())
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
		if(m_Velocity.y < 0.f)
			m_Velocity.y += m_GravityAccel * DT;
		else
			m_GravityVelocity.y += m_GravityAccel * DT;
		
		// 중력으로 인해 증가한 속도가 최대 제한치를 넘어서면, 맥스값으로 세팅
		if (m_MaxGravitySpeed < m_Velocity.y + m_GravityVelocity.y)
		{
			m_Velocity.y = 0.f;
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
