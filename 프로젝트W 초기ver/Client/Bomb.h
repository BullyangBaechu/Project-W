#pragma once
#include "Actor.h"
#include "Collider.h"
#include "CollisionMgr.h"
#include "Texture.h"

class Bomb :
    public Actor
{
private :
    // int m_HP;
    Texture* m_Tex = nullptr;

    class Collider* m_Collider;

    virtual void Render(HDC _dc) override;

public:
    void Init();
    virtual void Tick() override;
};

