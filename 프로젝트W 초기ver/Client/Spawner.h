#pragma once
#include "Actor.h"

enum class STAGEMODE
{
   RUN,
   INFINITERUN,
   CHALLENGE,

};

class Spawner :
    public Actor
{
private:
    float m_SpawnDelay = 2.f;           // 초기 스폰 간격 시간
    float m_MinSpawnDelay = 0.5f;       // 최단 스폰 간격
    float m_DifficultyTime = 0.f;       // 난이도 누적 시간
    float m_DelayDescreaseTime = 0.05f; // 몇 초마다 시간을 줄인걸지
    float m_AccTime = 0.f;              // 누적시간
    
    bool m_FirstSpawn = true;           // 첫 생성 오브젝트 고정 용
    

    // 디폴트 옵션
    STAGEMODE m_StageMode = STAGEMODE::RUN;

private:
    void SpawnRandomObject();


public:
    virtual void Tick() override;
    virtual void FinalTick() override;

    void SetSpawnDelay(float delay) { m_SpawnDelay = delay; }
    float GetSpawnDelay() const { return m_SpawnDelay; }
    GET_SET(STAGEMODE, StageMode);

    Spawner();
    ~Spawner();
   
};

