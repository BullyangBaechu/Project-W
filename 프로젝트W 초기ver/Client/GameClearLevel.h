#pragma once
#include "Level.h"

class Texture;
class Sound;


class GameClearLevel :
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
    GameClearLevel();
    ~GameClearLevel();

};

