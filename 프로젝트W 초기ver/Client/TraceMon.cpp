#include "pch.h"
#include "TraceMon.h"

#include "TimeMgr.h"
#include "LevelMgr.h"
#include "Monster.h"
#include "Player.h"

TraceMon::TraceMon()
{
}

TraceMon::~TraceMon()
{
}

void TraceMon::FinalTick()
{
	// 대상(플레이어)을 추적한다.
	Vec2 vPlayerPos = m_Player->GetPos();
	Vec2 vMonPos = m_Monster->GetPos();

	Vec2 vDir = vPlayerPos - vMonPos;

	if (vDir.Length() != 0.f)
	{
		// Monster 를 플레이어쪽으로 이동시킨다.	
		vMonPos += vDir.Normalize() * m_Monster->GetMoveSpeed() * DT;
		m_Monster->SetPos(vMonPos);
	}	

	// 추적하다가 MaxMoveRange 이상 움직였으면, 되돌아간다.

	// 추적하다가 공격범위 안쪽으로 대상이 들어오면, 공격상태로 전환

	// 공격을 받으면, Hurt 상태로 전환

}

void TraceMon::Enter()
{
	// IdleState 소유자가 Monster 인지 확인
	m_Monster = dynamic_cast<Monster*>(GetActor());
	assert(m_Monster);

	// 타겟 플레이어를 알아낸다.
	//m_Player = dynamic_cast<Player*>(LevelMgr::GetInst()->FindActorByName(ACTOR_TYPE::PLAYER, L"Player"));
	m_Player = dynamic_cast<Player*>(LevelMgr::GetInst()->GetPlayer());
	assert(m_Player);
}

void TraceMon::Exit()
{
}