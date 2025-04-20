/*
#include "pch.h"
#include "Spawner.h"
#include "Brick.h"
#include "Bomb.h"

#include "TimeMgr.h"
#include "Engine.h"
#include "Camera.h"
#include "LevelMgr.h"
#include "Level.h"
*/
/*
Spawner::Spawner()
{
}

Spawner::~Spawner()
{
}
/*
void Spawner::Tick()
{
	
	m_AccTime += DT;
	m_DifficultyTime += DT;

	// 오브젝트 스폰
	if (m_AccTime > m_SpawnDelay)
	{
		//OutputDebugString(L"[Spawner] 벽돌 생성 트리거\n");
		SpawnRandomObject();
		m_AccTime = 0;
	}

	// 일정 시간마다 스폰 간격 줄이기
	if (m_DifficultyTime >= 5.f)
	{
		m_DifficultyTime = 0.f;

		// 딜레이 줄이기 (단, 최소 한계 유지)
		m_SpawnDelay -= m_DelayDescreaseTime;
		if (m_SpawnDelay < m_MinSpawnDelay)
			m_SpawnDelay = m_MinSpawnDelay;

	}

	
}

void Spawner::FinalTick()
{

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

	// 첫 스폰 고정 시 사용할 조건문 (만약 안 쓴다면 if else 삭제 시키고 else에 있는 코드만 갖다 쓰면됨
	if (m_FirstSpawn)
	{
		Brick* brick = new Brick;
		brick->Init(1); // 무조건 Lv1
		object = brick;
		object->SetCamCheck(false);
		LevelMgr::GetInst()->GetCurrentLevel()->AddObject(ACTOR_TYPE::BRICK, object);

		m_FirstSpawn = false;
	}
	else
	{
		if (Random < 80)
		{
			Brick* brick = new Brick;

			// 벽돌 단계 별 가중치
			int weight[4] = { 50, 30, 15, 5 };
			int r = rand() % 100;
			int acc = 0;
			int type = 1;

			// 가중치에 맞춰 type 확률 조정
			for (int i = 0; i < 4;i++)
			{
				acc += weight[i];
				if (r < acc)
				{
					type = i + 1;
					break;
				}
			}

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
	}

	if (object)
	{

		// 벽돌, 폭탄을 오른쪽 상단 구석에 스폰시키기
		Vec2 CamPos = Camera::GetInst()->GetLookAt();
		Vec2 Res = Engine::GetInst()->GetResolution();

		float ObjectPos_x = CamPos.x + (Res.x / 2.f) - object->GetScale().x;
		float ObjectPos_y = CamPos.y - (Res.y / 2.f) + object->GetScale().y;


		Vec2 ObjectPos = Vec2(ObjectPos_x, ObjectPos_y);

		object->SetPos(ObjectPos);

	}
}
*/

