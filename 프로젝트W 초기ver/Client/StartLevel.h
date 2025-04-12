#pragma once
#include "Level.h"
class StartLevel :
    public Level
{
private:


public:
    virtual void Tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    StartLevel();
    ~StartLevel();
};

