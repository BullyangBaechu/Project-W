#pragma once
#include "Actor.h"
#include "Collider.h"
#include "CollisionMgr.h"
#include "Texture.h"

class Brick :
    public Actor
{
private :
    int m_MaxHP;            // 벽돌 최대 체력
    int m_CurHP;            // 벽돌 현재 체력

    Texture* m_Tex = nullptr;


    class Collider*     m_Collider;
    class RigidBody*    m_RigidBody;

    bool IsHit;             // 피격 판정

    // 파괴 용
    Texture* m_DestoryTex;
    float m_DestructionTimer;
    bool m_bDestroyed;

public:
    void Init(int type);    // type == 벽돌 종류
    void Hit(int dmg);      // 캐릭터 dmg

    void StartDestroy();        // 파괴 시 호출할 함수

    virtual void Tick() override;
    virtual void Render(HDC _dc) override;

    virtual void BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;
    virtual void EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;

};

