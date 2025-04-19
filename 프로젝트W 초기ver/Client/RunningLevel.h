#pragma once
#include "Level.h"
#include "Texture.h"
class RunningLevel :
    public Level
{
private:
    Texture* m_BGTex;

public:
    virtual void Enter() override;
    virtual void Tick()  override;
    virtual void Exit()  override;

    virtual void Render(HDC _dc) override;


public:
    RunningLevel();
    ~RunningLevel();
};

