#pragma once
#include "State.h"


class IdleMon :
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
    IdleMon();
    ~IdleMon();
};

