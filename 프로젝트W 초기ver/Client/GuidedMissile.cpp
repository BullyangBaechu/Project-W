#include "pch.h"
#include "GuidedMissile.h"

#include "LevelMgr.h"
#include "Level.h"

#include "TimeMgr.h"

GuidedMissile::GuidedMissile()
	: m_Target(nullptr)
	, m_DetectRange(500)
	, m_TargetType(ACTOR_TYPE::END)
	, m_RotateSpeed(0.f)
	, m_Time(0.f)
{
	SetRotateSpeed(90.f);
}

GuidedMissile::~GuidedMissile()
{
}

void GuidedMissile::Tick()
{
	FindNextTarget();

	Trace();

	ScreenCheck();

	m_Time += DT;

	if (m_Time > 5.f)
		Destroy();
}

void GuidedMissile::FindNextTarget()
{
	// 이미 추적중인 대상이 있으면, 탐색하지 않는다.
	if (nullptr != m_Target)
		return;

	// 현재 레벨에서 추적 대상 그룹 타입 물체들을 가져온다.
	Level* CurLevel = LevelMgr::GetInst()->GetCurrentLevel();
	const vector<Actor*>& vecActor = CurLevel->GetObjects(m_TargetType);

	// 유도탄 본인의 위치
	Vec2 vPos = GetPos();

	float fMinDist = m_DetectRange;

	for (size_t i = 0; i < vecActor.size(); ++i)
	{
		// 추적대상 후보의 위치
		Vec2 vTargetPos = vecActor[i]->GetPos();

		// 유도탄과 추적대상과의 거리값을 잰다.
		float fDist = vPos.Distance(vTargetPos);

		if (fMinDist < fDist)
			continue;

		fMinDist = fDist;	
		m_Target = vecActor[i];		
	}	
}

void GuidedMissile::Trace()
{	
	if (IsValid(m_Target))
	{
		// 미사일에서 목적지로 향하는 방향을 구한다.
		Vec2 vTargetDir = m_Target->GetPos() - GetPos();
		vTargetDir.Normalize();

		// 현재 진행방향을 알아낸다.
		Vec2 vCurDir = GetVelocity();
		vCurDir.Normalize();

		// 두 단위벡터를 내적해서 얻은값은 두 벡터사이의 각도 코사인값이다.
		// 두 벡터가 거의 일치한다는 것은 두 사이각도가 0에 가깝고, 코사인세타값은 1에 근사한 값이 나올 것이다.
		float fDot = vTargetDir.Dot(vCurDir);

		// 현재 방향이 목적지를 향하는 방향과 거의 일치한다면,
		// 두 방향벡터가 같은 것으로 본다.
		if (0.999f < fDot)
		{
			vCurDir = vTargetDir;						
			// 회전시킨 방향으로 속도를 설정한다.			
			SetVelocity(vCurDir * GetVelocity().Length());
		}

		else
		{
			float Dir = 1.f;
			// 회전 방향 계산
			if (vCurDir.x * vTargetDir.y - vCurDir.y * vTargetDir.x < 0.f)
			{
				Dir = -1.f;
			}

			// 방향벡터를 회전한다.
			float AddAngle = ((m_Time * 5.f) / 180.f) * PI;
			vCurDir = Rotate(vCurDir, (m_RotateSpeed + AddAngle) * DT * Dir);

			// 회전시킨 방향으로 속도를 설정한다.
			SetVelocity(vCurDir * GetVelocity().Length());
		}
	}

	Move();

	// 1.f, 1.f 방향의 벡터를 정규화해서 방향은 유지한채로 길이를 1로 만듬
	Vec2 V = Vec2(1.f, 1.f);
	V.Normalize();

	// 길이 1짜리 벡터를, 방향을 유지한채로 길이를 100으로 늘림
	V *= 100.f;

	// 45도 방향으로 길이 100짜리 벡터가 되기위한 x, y 값
	Vec2 Velocity = Vec2(70.7213f, -70.7213f);
}
