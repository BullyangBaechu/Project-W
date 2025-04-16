#pragma once
#include "Actor.h"
class Bomb :
    public Actor
{
private :
    // int m_HP;

    class Collider* m_Collider;

public:
    void Init();
    virtual void Tick() override;
};

