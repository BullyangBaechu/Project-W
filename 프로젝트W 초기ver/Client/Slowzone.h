#pragma once
#include "Actor.h"

#include "Collider.h"
#include "CollisionMgr.h"
#include "Texture.h"
#include "RigidBody.h"

class Slowzone :
    public Actor
{
private :
    float m_SlowRatio;      // 감속비율

    Collider* m_Collider;
    //RigidBody* m_RigidBody;
    Texture* m_Tex;


public :
    void Init();

    virtual void Tick() override;
    virtual void Render(HDC _dc) override;

    float GetSlowRatio() { return m_SlowRatio; }
    void SetSlowRatio(float _ratio) { m_SlowRatio = _ratio; }


    virtual void BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;
    virtual void EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;

public:
    Slowzone();
    ~Slowzone();
};

