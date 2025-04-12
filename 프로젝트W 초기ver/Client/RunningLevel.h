#pragma once
#include "Level.h"
class RunningLevel :
    public Level
{
public:
    virtual void Enter() override;
    virtual void Tick()  override;
    virtual void Exit()  override;


public:
    RunningLevel();
    ~RunningLevel();
};

