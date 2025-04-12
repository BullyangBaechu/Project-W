#pragma once
#include "Component.h"


class Collider :
    public Component
{
private:
    Vec2        m_OffsetPos;    // 오브젝트로 부터 상대적인 위치(상대 좌표)
    Vec2        m_Scale;        // 충돌체의 크기
    Vec2        m_FinalPos;     // 충돌체의 최종 좌표, (본체(Actor) 의 위치 + m_OffsetPos) 

    int         m_Overlap;      // 다른 충돌체와 중첩(겹침) 상태를 카운팅한다.


public:
    void SetOffset(Vec2 _Offset) { m_OffsetPos = _Offset; }
    void SetScale(Vec2 _Scale) { m_Scale = _Scale; }

    Vec2 GetOffset() { return m_OffsetPos; }
    Vec2 GetScale() { return m_Scale; }

    Vec2 GetFinalPos() { return m_FinalPos; }
     
public:
    virtual void FinalTick() override;

public:
    // 중첩이 시작된느 순간 호출되는 함수
    void BeginOverlap(Collider* _Other);

    // 중첩되어있으면 호출되는 함수
    void Overlap(Collider* _Other); 

    // 중첩상태를 벗어나는 순간 호출되는 함수
    void EndOverlap(Collider* _Other);

public:
    Collider();
    virtual ~Collider();
};

