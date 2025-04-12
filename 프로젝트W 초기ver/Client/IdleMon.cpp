#include "pch.h"
#include "IdleMon.h"

#include "LevelMgr.h"
#include "Monster.h"
#include "Player.h"

IdleMon::IdleMon()
	: m_Monster(nullptr)
	, m_Player(nullptr)
{
}

IdleMon::~IdleMon()
{
}

void IdleMon::FinalTick()
{
	// 플레이어와 Monster 의 거리를 체크해서, DetectRange 이내에 있는지 확인한다.
	float fDist = m_Player->GetPos().Distance(m_Monster->GetPos());

	// 만약 m_DetectRage 안에 플레이어가 들어오면 자신의 상태를 Trace 상태로 변경한다.
	if (fDist < m_Monster->GetDetectRange())
	{
		GetOwnerSM()->ChangeState(L"TraceMon");
	}

	DrawDebugCircle(m_Monster->GetPos(), m_Monster->GetDetectRange(), PEN_TYPE::GREEN, 0.f);
}

void IdleMon::Enter()
{
	// IdleState 소유자가 Monster 인지 확인
	m_Monster = dynamic_cast<Monster*>(GetActor());
	assert(m_Monster);

	// 타겟 플레이어를 알아낸다.
	//m_Player = dynamic_cast<Player*>(LevelMgr::GetInst()->FindActorByName(ACTOR_TYPE::PLAYER, L"Player"));
	m_Player = dynamic_cast<Player*>(LevelMgr::GetInst()->GetPlayer());
	assert(m_Player);
}

void IdleMon::Exit()
{
}