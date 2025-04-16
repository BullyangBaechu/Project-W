#pragma once
#include "Actor.h"

class Spawner :
    public Actor
{
private:
    float m_SpawnDelay = 1.f;
    float m_AccTime = 0.f;

private:
    void SpawnRandomObject();

public:
    virtual void Tick() override;

    Spawner();
    ~Spawner();
   
};

