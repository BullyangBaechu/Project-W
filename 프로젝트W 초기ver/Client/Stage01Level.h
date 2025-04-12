#pragma once
#include "Level.h"
class Stage01Level :
    public Level
{

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    Stage01Level();
    ~Stage01Level();
};

