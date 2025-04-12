#pragma once
#include "Actor.h"


class Ground :
    public Actor
{
private:
    class Collider* m_Collider;


public:
    virtual void Tick() override;


public:
    Ground();
    ~Ground();
};

