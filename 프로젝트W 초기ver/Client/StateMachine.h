#pragma once
#include "Component.h"

class State;

class StateMachine :
    public Component
{
private:
    map<wstring, State*>    m_mapState;
    State*                  m_CurState;
    State*                  m_PrevState;

public:
    void AddState(const wstring& _Key, State* _State);
   

    State* GetState(const wstring& _Key)
    {
        map<wstring, State*>::iterator iter = m_mapState.find(_Key);
        if (iter == m_mapState.end())
            return nullptr;
        return iter->second;
    }

    void ChangeState(const wstring& _NextState);
    void ChangePrevState();

public:
    virtual void FinalTick() override;

public:
    StateMachine();
    ~StateMachine();
};

