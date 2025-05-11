#pragma once
#include "Actor.h"
#include "Texture.h"
#include "Sound.h"

class EffectActor :
    public Actor
{
private:
    Texture* m_Tex;
    Sound* m_Sound;
    Actor* m_Target;
    Vec2   m_RelativeOffset = Vec2(0.f, 0.f); // 따라붙을 상대 위치

    float m_LifeTime;
    float m_Timer;

public:
    void SetEffect(Texture* _tex, float _lifetime, Sound* _sound = nullptr);
    void SetFollowTarget(Actor* _target, Vec2 _offset)
    {
        m_Target = _target;
        m_RelativeOffset = _offset;
    }
    virtual void Tick() override;
    virtual void Render(HDC _dc) override;


public:
    EffectActor();
    ~EffectActor();
};

