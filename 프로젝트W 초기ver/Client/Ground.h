#pragma once
#include "Actor.h"

class Texture;
class Collider;

class Ground :
    public Actor
{
private:
    Collider* m_Collider;
    Texture*    m_Tex;

    int BGidx;              // 반복시킬 Ground 텍스쳐 수

public:
    virtual void Tick() override;
    virtual void Render(HDC _dc) override;


public:
    Ground();
    ~Ground();
};

