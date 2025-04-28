#pragma once
#include "Actor.h"
#include "Collider.h"
#include "CollisionMgr.h"
#include "Texture.h"
#include "Player.h"

class Bomb :
    public Actor
{
private :
    // int m_HP;
    Texture* m_Tex = nullptr;
    Collider* m_Collider;

    // 추적
    Player* m_Target;           // 추적 타겟 = player
    float m_speed;
    
public:
    void SetTarget(Player* _player) { m_Target = _player; }

public:
    void Init();
    virtual void Tick() override;
    virtual void Render(HDC _dc) override;

    virtual void BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;
    virtual void Overlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;
    virtual void EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;
};

