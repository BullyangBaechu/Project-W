#pragma once
#include "Actor.h"

class Spawner :
    public Actor
{
private:
    float m_SpawnDelay = 1.f;
    float m_AccTime = 0.f;

    //vector<Actor*> m_PendingAdd;

private:
    void SpawnRandomObject();


public:
    virtual void Tick() override;
    virtual void FinalTick() override;

    void SetSpawnDelay(float delay) { OutputDebugString(L"[Spawner] SetSpawnDelay »£√‚µ \n"); m_SpawnDelay = delay; }
    float GetSpawnDelay() const { return m_SpawnDelay; }

    Spawner();
    ~Spawner();
   
};

