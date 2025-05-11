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

    float m_LifeTime;
    float m_Timer;

public:
    void SetEffect(Texture* _tex, float _lifetime, Sound* _sound = nullptr);
    
    virtual void Tick() override;
    virtual void Render(HDC _dc) override;


public:
    EffectActor();
    ~EffectActor();
};

