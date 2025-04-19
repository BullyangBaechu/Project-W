#pragma once
#include "Level.h"
class InfiniteLevel :
    public Level
{
public:
    virtual void Enter() override;
    virtual void Tick()  override;
    virtual void Exit()  override;


public:
    InfiniteLevel();
    ~InfiniteLevel();
};

