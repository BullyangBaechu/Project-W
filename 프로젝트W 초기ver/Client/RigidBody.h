#pragma once
#include "Component.h"

enum class RIGIDBODY_MODE
{
    TOPVIEW,
    BELTSCROLL,
};

class RigidBody :
    public Component
{
private:
    float           m_Mass;         // 질량
    Vec2            m_Force;        // 누적된 힘
    Vec2            m_Velocity;     // 속력 + 이동방향
    float           m_Friction;     // 마찰계수

    float           m_InitSpeed;    // 초기속력 (정지 -> 이동) 으로 변환될때
    float           m_MaxSpeed;     // 최대속력

    bool            m_Move;

    RIGIDBODY_MODE  m_RigidBodyMode;

    // BeltScroll
    Vec2            m_GravityVelocity;
    float           m_MaxGravitySpeed;
    float           m_GravityAccel;
    bool            m_Ground;
    bool            IsJump;

    float           m_JumpSpeed; 
    int             m_MaxJumpStack;
    int             m_CurJumpStack;



public:
    GET_SET(float, Mass);
    GET_SET(Vec2, Force);
    GET_SET(Vec2, Velocity);
    GET_SET(float, Friction);
    GET_SET(float, InitSpeed);
    GET_SET(float, MaxSpeed);
    GET_SET(RIGIDBODY_MODE, RigidBodyMode);
    GET_SET(float, JumpSpeed);

    void SetGround(bool _Ground) 
    { 
        m_Ground = _Ground; 
        if (m_Ground)
        {
            m_CurJumpStack = 0;
        }
    }
    bool GetGround() { return m_Ground; }


    void AddForce(Vec2 _Force) { m_Force += _Force; }
    void Jump();

public:
    virtual void FinalTick() override;

private:
    //void TopView();
    void BeltScroll();


public:
    RigidBody();
    ~RigidBody();
};

