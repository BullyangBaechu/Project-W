#pragma once
#include "Level.h"
#include "Texture.h"
class RunningLevel :
    public Level
{
private:
    Texture* m_BGTex;               // 배경 텍스쳐

public:
    virtual void Enter() override;
    virtual void Tick()  override;
    virtual void Exit()  override;

    virtual void Render(HDC _dc) override;


public:
    RunningLevel();
    ~RunningLevel();
};

