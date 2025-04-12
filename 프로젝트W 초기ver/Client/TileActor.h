#pragma once
#include "Actor.h"

class TileMap;

class TileActor :
    public Actor
{
private:
    TileMap*    m_TileMap;

public:
    void ShowGrid(bool _Show);
    

    

public:
    virtual void Tick() override;
    virtual void Render(HDC _dc) override;
    virtual bool CamCheck() override { return true; }

public:
    TileActor();
    ~TileActor();
};

