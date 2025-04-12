#pragma once
#include "Base.h"

#include "Actor.h"

class Component :
    public Base
{
private:
    const COMPONENT_TYPE    m_Type;  // 컴포넌트 타입
    Actor*                  m_Actor; // 컴포넌트를 소유하고있는 Actor 의 주소
     
public:
    Actor* GetOwner() { return m_Actor; }
    COMPONENT_TYPE GetType() { return m_Type; }

public:
    virtual void FinalTick() = 0;

public:
    Component(COMPONENT_TYPE _Type);
    virtual ~Component();

    friend class Actor;
};

