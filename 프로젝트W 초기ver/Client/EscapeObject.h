#pragma once
#include "Actor.h"

class  Texture;

class EscapeObject :
    public Actor
{
private:
    Texture* m_tex;

public:
    virtual void Tick() override;
    virtual void Render(HDC _dc) override;

public:
    EscapeObject();
    ~EscapeObject();
};

