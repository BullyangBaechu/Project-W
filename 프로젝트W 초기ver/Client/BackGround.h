#pragma once
#include "Actor.h"
#include "Texture.h"

class BackGround :
    public Actor
{
private:

    Texture* m_Tex;

    float m_ParallaxFactor; // 배경 원근감을 주기 위한 배경 속도 비율
   
    Vec2 m_InitialPos; // 배경 첫 위치

public:

    void SetParallaxFactor(float factor) { m_ParallaxFactor = factor; }

    virtual void Tick() override;
    virtual void Render(HDC _dc) override;

    BackGround();
    ~BackGround();

};
