#pragma once
#include "Level.h"

class Texture;

class StartLevel :
    public Level
{
private:
    Texture* m_tex;


public:
    virtual void Tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void Render(HDC _dc) override;

public:
    StartLevel();
    ~StartLevel();
};

