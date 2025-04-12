#pragma once
#include "Level.h"
class TestLevel :
    public Level
{
public:
    virtual void Enter() override;
    virtual void Tick()  override;
    virtual void Exit()  override;


public:
    TestLevel();
    ~TestLevel();

};

