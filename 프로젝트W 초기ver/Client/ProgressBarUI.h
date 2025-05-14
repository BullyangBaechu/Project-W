#pragma once
#include "UI.h"
class ProgressBarUI :
    public UI
{
private:
    float m_Progress;
    UI* m_FillBar;

public:
    virtual void Tick_UI() override;
    virtual void Render_UI(HDC _dc) override;

    void SetProgress(float percent) { m_Progress = percent; }
    void SetFillBar(UI* pUI) { m_FillBar = pUI; }

public:
    ProgressBarUI();
    ~ProgressBarUI();
};

