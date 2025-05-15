#pragma once
#include "Level.h"

class Texture;

class GameOverLevel :
    public Level
{
private:
    Texture* m_tex;

public:
    virtual void Enter() override;
    virtual void Tick()  override;
    virtual void Exit()  override;

    virtual void Render(HDC _dc) override;


public:
    GameOverLevel();
    ~GameOverLevel();
};

