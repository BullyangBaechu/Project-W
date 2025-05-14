#pragma once
#include "Level.h"
#include "Texture.h"

class ProgressBarUI;

class RunningLevel :
    public Level
{
private:
    Texture* m_BGTex;               // 배경 텍스쳐
    bool m_bFirstFrame = true;      // 첫 프레임

    float m_GoalX;

    ProgressBarUI* m_ProgressBar;   // 진행도 게이지 UI

public:
    virtual void Enter() override;
    virtual void Tick()  override;
    virtual void Exit()  override;

    virtual void Render(HDC _dc) override;


public:
    RunningLevel();
    ~RunningLevel();
};

