#pragma once
#include "Actor.h"

class Texture;
class Collider;
class FlipbookPlayer;
class RigidBody;


enum class PLAYER_ANIM
{
    IDLE_UP,
    IDLE_DOWN,
    IDLE_LEFT,
    IDLE_RIGHT,

    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
};


class Player :
    public Actor
{
private:
    float           m_Speed;    // 플레이어의 이동속력    
    Texture*        m_Texture;
       
    Collider*       m_HurtBox;
    FlipbookPlayer* m_FBPlayer;
    RigidBody*      m_RigidBody;




public:
    virtual void Tick() override;   
    virtual void Render(HDC _dc) override;
    virtual void BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;
    virtual void EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;

    virtual bool CamCheck() override;

private:
    void Shoot();
    void ChangeFlipbook();

public:
    Player();
    ~Player();
};

