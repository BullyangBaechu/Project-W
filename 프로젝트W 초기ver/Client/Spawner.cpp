#include "pch.h"
#include "Spawner.h"
#include "Brick.h"
#include "Bomb.h"

#include "TimeMgr.h"
#include "Engine.h"

Spawner::Spawner()
{
}

Spawner::~Spawner()
{
}

void Spawner::Tick()
{
	m_AccTime += DT;

	if (m_AccTime > m_SpawnDelay)
	{
		SpawnRandomObject();
		m_AccTime = 0;
	}


	//Actor::FinalTick();
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
	}
	else
	{
		Bomb* bomb = new Bomb;
		bomb->Init();
		object = bomb;
	}

	if (object)
	{
		// 벽돌, 폭탄을 오른쪽 상단 구석에 스폰시키기
		Vec2 Pos = Engine::GetInst()->GetResolution();
		
		float ObjectPos_x = Pos.x - object->GetScale().x;
		float ObjectPos_y = 10.f + object->GetScale().y;

		/*
		상단 정확히 모서리에 고정 시키려면 이거 써야지
		float ObjectPos_x = Pos.x - (object->GetScale().x / 2.f);
		float ObjectPos_y = (object->GetScale().y / 2.f);
		*/
		Vec2 ObjectPos = Vec2(ObjectPos_x, ObjectPos_y);

		object->SetPos(ObjectPos);

		AddChild(object);
	}
}

