#pragma once
#include "UI.h"

class PanelUI :
    public UI
{
private:
    Vec2    m_LBtnPressedPos;



public:
    void Tick_UI() override;
    void Render_UI(HDC _dc) override;

    virtual void BeginHover() override;
    virtual void EndHover() override;

    virtual void LbtnDown() override;
    virtual void LbtnUP() {}
    virtual void LbtnClicked();

public:
    PanelUI();
    ~PanelUI();
};

