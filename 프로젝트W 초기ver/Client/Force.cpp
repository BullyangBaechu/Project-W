#include "pch.h"
#include "Force.h"

#include "LevelMgr.h"
#include "Level.h"
#include "TimeMgr.h"

#include "RigidBody.h"

Force::Force()
	: m_ForceScale(0.f)
	, m_Time(0.f)
	, m_Duration(0.f)
	, m_ActorType(0)
{

}

Force::~Force()
{
}

void Force::Tick()
{
	for (UINT i = 0; i < (UINT)ACTOR_TYPE::END;++i)
	{
		if (!(m_ActorType & (1 << i)))
			continue;

		Level* pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
		const vector<Actor*>& vecActor = pCurLevel->GetObjects((ACTOR_TYPE)i);

		for (size_t j = 0; j < vecActor.size(); ++j)
		{
			RigidBody* pRigidBody = vecActor[j]->GetComponent<RigidBody>();
			if (!pRigidBody)
				continue;

			float fDistance = GetPos().Distance(vecActor[j]->GetPos());

			if (m_Range < fDistance)
				continue;

			// 힘이 적용될 방향
			Vec2 vForceDir = vecActor[j]->GetPos() - GetPos();
			vForceDir.Normalize();

			// 거리에 따른 힘이 적용될 비율
			float ForceRatio = 1.f - (fDistance / m_Range);

			// 물체에 가해질 최종 힘
			Vec2 vForce = vForceDir * m_ForceScale * ForceRatio;

			pRigidBody->AddForce(vForce);
		}
	}



	DrawDebugCircle(GetRenderPos(), m_Range, PEN_TYPE::BLUE, 0.f);

	m_Time += DT;
	if (m_Duration < m_Time)
		Destroy();
}
