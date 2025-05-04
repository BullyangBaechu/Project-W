#pragma once
#include "Actor.h"

class Texture;
class Collier;

class Slowzone :
    public Actor
{
private :
    float m_SlowRatio;      // 감속비율

    Collider* m_Collider;
    Texture* m_Tex;


public :
    void Init();

    virtual void Tick() override;
    virtual void Render(HDC _dc) override;

    float GetSlowRatio() { return m_SlowRatio; }
    void SetSlowRatio(float _ratio) { m_SlowRatio = _ratio; }

public:
    Slowzone();
    ~Slowzone();
};

