#pragma once
#include "Missile.h"

class GuidedMissile :
    public Missile
{
private:
    Actor*      m_Target;
    ACTOR_TYPE  m_TargetType;   // 타겟 후보 그룹
    float       m_DetectRange;  // 타겟을 인지할 수 있는 탐지 범위

    float       m_RotateSpeed;  // 방향회전속도

    float       m_Time;         // 누적시간

public:
    void SetTargetActorType(ACTOR_TYPE _TargetType) { m_TargetType = _TargetType; }
    ACTOR_TYPE GetTargetActorType() { return m_TargetType; }

    void SetDetectRange(float _Range) { m_DetectRange = _Range; }
    float GetDetectRange() { return m_DetectRange; }

    void SetRotateSpeed(float _Degree) { m_RotateSpeed = _Degree * (PI / 180.f); }


public:
    virtual void Tick() override;

private:
    void FindNextTarget();
    void Trace();

public:
    GuidedMissile();
    ~GuidedMissile();
};

