#pragma once
#include "Level.h"

class Texture;
class Sound;

class GameOverLevel :
    public Level
{
private:
    Texture* m_tex;
    Sound* m_bgm;

public:
    virtual void Enter() override;
    virtual void Tick()  override;
    virtual void Exit()  override;

    virtual void Render(HDC _dc) override;


public:
    GameOverLevel();
    ~GameOverLevel();
};

