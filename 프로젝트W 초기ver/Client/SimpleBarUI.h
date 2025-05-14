#pragma once
#include "UI.h"
class SimpleBarUI :
    public UI
{
public:
    virtual void Tick_UI() override {}
    virtual void Render_UI(HDC _dc) override;
};

