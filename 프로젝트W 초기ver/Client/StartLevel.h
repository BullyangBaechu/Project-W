#pragma once
#include "Level.h"

class Texture;
class Sound;

class StartLevel :
    public Level
{
private:
    Texture* m_tex;
    Sound* m_bgm;

public:
    virtual void Tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void Render(HDC _dc) override;

public:
    StartLevel();
    ~StartLevel();
};

