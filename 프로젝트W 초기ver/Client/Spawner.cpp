#include "pch.h"
#include "Spawner.h"
#include "Brick.h"
#include "Bomb.h"

#include "TimeMgr.h"
#include "Engine.h"
#include "Camera.h"
#include "LevelMgr.h"
#include "Level.h"

Spawner::Spawner()
{
}

Spawner::~Spawner()
{
	/*
	for (size_t i = 0; i < m_ChildActor.size(); ++i)
	{
		if (nullptr != m_ChildActor[i])
		{
			delete m_ChildActor[i];
			m_ChildActor[i] = nullptr;
		}
	}
	m_ChildActor.clear();
	*/
}

void Spawner::Tick()
{
	OutputDebugString(L"[Spawner] Tick 호출됨\n");
	wchar_t sz[100];
	swprintf_s(sz, 100, L"[Spawner] Acc: %.3f / Delay: %.3f\n", m_AccTime, m_SpawnDelay);
	OutputDebugString(sz);

	m_AccTime += DT;

	if (m_AccTime > m_SpawnDelay)
	{
		OutputDebugString(L"[Spawner] 벽돌 생성 트리거\n");
		SpawnRandomObject();
		m_AccTime = 0;
	}
	
}

void Spawner::FinalTick()
{
	// 다음 프레임부터 처리
	//for (Actor* obj : m_PendingAdd)
		//AddChild(obj);
	//m_PendingAdd.clear();

	// 자식 FinalTick 호출 (컴포넌트 + 파괴 처리)
	for (Actor* child : m_ChildActor)
		child->FinalTick();

	// 자식 Tick 수동 호출
	for (Actor* child : m_ChildActor)
		child->Tick();

	// Spawner 자신의 컴포넌트 FinalTick
	Actor::FinalTick();
}

void Spawner::SpawnRandomObject()
{
	Actor* object = nullptr;
	int Random = rand() % 100;

	if (Random < 80)
	{
		Brick* brick = new Brick;
		int type = rand() % 4 + 1;
		brick->Init(type);
		object = brick;
		object->SetCamCheck(false);
		//LevelMgr::GetInst()->GetCurrentLevel()->AddObject(ACTOR_TYPE::BRICK, brick);
		LevelMgr::GetInst()->GetCurrentLevel()->AddObject(ACTOR_TYPE::BRICK, (Actor*)brick);
	}
	else
	{
		Bomb* bomb = new Bomb;
		bomb->Init();
		object = bomb;
		object->SetCamCheck(false);
		LevelMgr::GetInst()->GetCurrentLevel()->AddObject(ACTOR_TYPE::BOMB, (Actor*)bomb);
	}

	if (object)
	{
		
		// 벽돌, 폭탄을 오른쪽 상단 구석에 스폰시키기
		Vec2 CamPos = Camera::GetInst()->GetLookAt();
		Vec2 Res = Engine::GetInst()->GetResolution();
		
		float ObjectPos_x = CamPos.x + (Res.x / 2.f) - object->GetScale().x;
		float ObjectPos_y = CamPos.y - (Res.y / 2.f) + object->GetScale().y;

		/*
		상단 정확히 모서리에 고정 시키려면 이거 써야지
		float ObjectPos_x = Pos.x - (object->GetScale().x / 2.f);
		float ObjectPos_y = (object->GetScale().y / 2.f);
		*/
		Vec2 ObjectPos = Vec2(ObjectPos_x, ObjectPos_y);

		object->SetPos(ObjectPos);
		// m_PendingAdd.push_back(object);
		// AddChild(object);
	}
}

