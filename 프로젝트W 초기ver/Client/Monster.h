#pragma once
#include "Actor.h"


class Monster :
    public Actor
{
private:
    class Texture*      m_Texture;
    class Collider*     m_Collider;
    int                 m_HP;

    Vec2                m_InitPos;      // 처음 생성 위치
    float               m_DetectRange;  // 인지범위
    float               m_AttRange;     // 공격 범위
    float               m_MaxMoveRange; // 최대 이동량    
    float               m_MoveSpeed;

    class StateMachine* m_SM;


public:
    void SetInitPos(Vec2 _InitPos) { m_InitPos = _InitPos; }
    GET_SET(float, DetectRange);
    GET_SET(float, AttRange);
    GET_SET(float, MaxMoveRange);
    GET_SET(float, MoveSpeed);

public:    
    virtual void Tick() override;
    virtual void Render(HDC _dc) override;
    virtual void BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;


public:
    Monster();
    ~Monster();
};

