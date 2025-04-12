#pragma once
#include "Base.h"

#include "StateMachine.h"
#include "Actor.h"

class State :
    public Base
{
private:
    StateMachine*   m_Owner;

public:
    StateMachine* GetOwnerSM() { return m_Owner; }
    Actor* GetActor() { return m_Owner->GetOwner(); }

public:
    virtual void FinalTick() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

public:
    State();
    ~State();

    friend class StateMachine;
};

