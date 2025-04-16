#pragma once
#include "Actor.h"
class Brick :
    public Actor
{
private :
    int m_MaxHP;            // 벽돌 최대 체력
    int m_CurHP;            // 벽돌 현재 체력

    class Collider*     m_Collider;
    class RigidBody*    m_RigidBody;

public:
    void Init(int type);    // type == 벽돌 종류
    void Hit(int dmg);      // 캐릭터 dmg

    virtual void Tick() override;

};

