#include "pch.h"
//#include "SpawnMgr.h"
#include "SpawnMgr.h"
#include "Brick.h"
#include "Bomb.h"
#include "LevelMgr.h"
#include "Level.h"
#include "TimeMgr.h"
#include "Camera.h"
#include "Engine.h"

SpawnMgr::SpawnMgr()
{
}

SpawnMgr::~SpawnMgr()
{
}

void SpawnMgr::Init()
{
    m_SpawnDelay = 1.f;
    m_MinSpawnDelay = 0.5f;
    m_DifficultyTime = 0.f;
    m_DelayDescreaseTime = 0.05f;
    m_AccTime = 0.f;
    m_FirstSpawn = true;
    m_StageMode = STAGEMODE::RUN;
}

void SpawnMgr::Tick()
{
    m_AccTime += DT;
    m_DifficultyTime += DT;

    if (m_AccTime >= m_SpawnDelay)
    {
        SpawnRandomObject();
        m_AccTime = 0.f;
    }

    if (m_StageMode == STAGEMODE::RUN && m_DifficultyTime >= 5.f)
    {
        m_DifficultyTime = 0.2f;
        m_SpawnDelay -= m_DelayDescreaseTime;

        // 스폰 간격 최소까지만 낮추기
        if (m_SpawnDelay < m_MinSpawnDelay)
            m_SpawnDelay = m_MinSpawnDelay;
    }
}

void SpawnMgr::SpawnRandomObject()
{
    Actor* object = nullptr;

    if (m_FirstSpawn)
    {
        Brick* brick = new Brick;
        brick->Init(1);  // Lv1 고정
        object = brick;
        object->SetCamCheck(false);
        LevelMgr::GetInst()->GetCurrentLevel()->AddObject(ACTOR_TYPE::BRICK, object);

        m_FirstSpawn = false;
    }
    else
    {
        int randVal = rand() % 100;

        if (randVal < 80)
        {
            // Brick 생성
            Brick* brick = new Brick;
            int r = rand() % 100;
            int acc = 0, type = 1;
            int weight[4] = { 50, 30, 15, 5 };

            if (m_StageMode == STAGEMODE::INFINITERUN)
            {
                // 무한모드면 가중치 동일          
                for (int i = 0; i < 4;i++)
                {
                    weight[i] = 25;
                }
            }

            // 가중치에 맞게 type을 결정하는 로직
            for (int i = 0; i < 4; ++i)
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
            LevelMgr::GetInst()->GetCurrentLevel()->AddObject(ACTOR_TYPE::BRICK, (Actor*)brick);
        }
        else
        {
            // Bomb 생성
            Bomb* bomb = new Bomb;
            bomb->Init();
            object = bomb;
            LevelMgr::GetInst()->GetCurrentLevel()->AddObject(ACTOR_TYPE::BOMB, (Actor*)bomb);
        }

        //if (object)
            //object->SetCamCheck(false);
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
