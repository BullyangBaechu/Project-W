#pragma once
#include "Actor.h"


class Force :
    public Actor
{
private:
    float       m_ForceScale;   // 힘의 크기
    float       m_Range;        // 힘이 적용되는 범위
    float       m_Time;         // 누적시간
    float       m_Duration;     // 유지시간
    UINT        m_ActorType;    // 영향을 받을 Actor 종류   

public:
    GET_SET(float, ForceScale);
    GET_SET(float, Range);
    GET_SET(float, Duration);

    void SetActorType(ACTOR_TYPE _Type, bool _True)
    {
        if (_True)
            m_ActorType |= (1 << (UINT)_Type);
        else
            m_ActorType &= ~(1 << (UINT)_Type);
    }

public:
    virtual void Tick() override;

public:
    Force();
    ~Force();
};

