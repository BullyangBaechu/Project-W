#pragma once
#include "State.h"
class TraceMon :
    public State
{
private:
    class Monster* m_Monster;
    class Player*  m_Player;

private:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    TraceMon();
    ~TraceMon();
};

