#pragma once
#include "Component.h"

class Flipbook;

class FlipbookPlayer :
    public Component
{
private:
    vector<Flipbook*>   m_vecFlipbook;  // 재생시킬 플립북 모음
    Flipbook*           m_CurFlipbook;  // 현재 재생중인 플립북
    int                 m_CurSprite;    // 플립북내에서 몇번 Sprite 를 화면에 띄울것인지
    float               m_FPS;          // 재생속도(초당 프레임 수)
    UINT                m_RepeatCount;  // 총 재생횟수
    UINT                m_PlayCount;    // 현재 재생횟수
    bool                m_Finish;       // 재생 종료 알림
    float               m_AccTime;      // 누적시간 저장(시간체크용)

public:
    void AddFlipbook(Flipbook* _Flipbook, int _Index = -1);
    Flipbook* GetFlipbook(int _idx) { return m_vecFlipbook[_idx]; }
    void Play(int _Idx, float _FPS = 30.f, int _StartSpriteIdx = 0, UINT _Repeat = 0);
    
    Flipbook* GetCurFlipbook() { return m_CurFlipbook; }
    int GetCurSpriteIdx() { return m_CurSprite; }
public:
    void FinalTick() override;
    void Render(HDC _dc);

public:
    FlipbookPlayer();
    ~FlipbookPlayer();
};

