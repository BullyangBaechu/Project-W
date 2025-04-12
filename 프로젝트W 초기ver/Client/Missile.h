#pragma once
#include "Actor.h"

class Collider;

class Missile :
    public Actor
{
private:
    Collider*   m_Collider;

    float       m_Speed;    // 미사일 속력
    float       m_Angle;    // 발사 각도

    float       m_Time;     // 시간 누적
    
    Vec2        m_Velocity; // 이동속도(속력 + 방향)

public:
    void SetDegree(float _Degree) { m_Angle = _Degree * (PI / 180.f); }
    void SetRadian(float _Radian) { m_Angle = _Radian; }
    float GetAngle() const { return (m_Angle / PI) * 180.f; }

    void SetSpeed(float _Speed){  m_Speed = _Speed; }
    float GetSpeed() const { return m_Speed; }

    void SetVelocity(Vec2 _Velocity) { m_Velocity = _Velocity; }
    Vec2 GetVelocity() { return m_Velocity; }

public:
    virtual void Tick() override;
    virtual void Render(HDC _dc) override;
    virtual void BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;

protected:
    void Move();
    void ScreenCheck();

public:
    Missile();
    ~Missile();
};