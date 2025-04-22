#pragma once
#include "Actor.h"

class Texture;
class Collider;

class Ground :
    public Actor
{
private:
    Collider* m_Collider;
    Texture*    m_Texture;

public:
    virtual void Tick() override;


public:
    Ground();
    ~Ground();
};

