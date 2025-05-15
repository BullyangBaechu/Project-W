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

    CYBORG_RUN,
    CYBORG_ATTACK,
    CYBORG_GUARD,
    CYBORG_HIT,

};


class Player :
    public Actor
{
private:
    float           m_Speed;            // 플레이어의 이동속력  
    float           m_SpeedMultiplier;  // 감속장판 계산을 위해 쓸 스탯
    bool m_IsInSlowzone = false;        // 감속장판 안에 있는지 여부
    Texture*        m_Texture;
       
    Collider*       m_HurtBox;
    FlipbookPlayer* m_FBPlayer;
    RigidBody*      m_RigidBody;

    // PlayerAttack 용
    Collider*       m_AttackCollider;   // 공격 판정 용
    float           m_AttackTimer;      // 공격 판정 타이머
    int             m_Dmg;              // 플레이어 공격력


    // PlayerGuard 용
    Collider*       m_GuardCollider;    // 가드 판정 용
    float           m_GuardTimer;       // 가드 판정 타이머
   
    Vec2            m_PrevPos;          // 이전 프레임 위치

    
    // Player 레벨 관련 스탯
    int             m_Level;
    float           m_exp;
    int             m_MaxHP;
    
    // 피격 판정 용
    bool            m_IsHit;
    float           m_HitTimer;
    float           m_HitDuration;


    

    //bool            m_BlockLeft;;
    //bool            m_BlockRight;

public:
    virtual void Tick() override;   
    virtual void Render(HDC _dc) override;
    virtual void BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;
    virtual void Overlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;
    virtual void EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) override;

    int GetPlayerDmg() { return m_Dmg; }
    int GetHP() { return m_MaxHP; }

    virtual bool CamCheck() override;

private:
    void PlayerGetExp();
    //void Shoot();
    void PlayerJump();
    void PlayerAttack();
    void PlayerGuard();
    void ChangeFlipbook();
    
    


public:
    Player();
    ~Player();
};

