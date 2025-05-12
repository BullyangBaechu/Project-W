#include "pch.h"
#include "EffectActor.h"
#include "TimeMgr.h"
#include "AssetMgr.h"
#include "Engine.h"

EffectActor::EffectActor()
{
}

EffectActor::~EffectActor()
{
}

void EffectActor::SetEffect(Texture* _tex, float _lifetime, Sound* _sound)
{
    m_Tex = _tex;
    m_LifeTime = _lifetime;
    m_Timer = _lifetime;
    m_Sound = _sound;

    if (m_Tex)
        SetScale(Vec2((float)m_Tex->GetWidth(), (float)m_Tex->GetHeight()));

    if (m_Sound)
        m_Sound->Play(false);
}

void EffectActor::Tick()
{
    if (m_Target)
    {
        SetPos(m_Target->GetPos() + m_RelativeOffset); // 계속 따라가기
    }

    m_Timer -= DT;
    if (m_Timer <= 0.f)
        Destroy();
}

void EffectActor::Render(HDC _dc)
{
    if (!m_Tex)
        return;

    Vec2 renderPos = GetRenderPos();

    TransparentBlt(_dc,
        (int)(renderPos.x - m_Tex->GetWidth() / 2.f),
        (int)(renderPos.y - m_Tex->GetHeight() / 2.f),
        (int)m_Tex->GetWidth(), (int)m_Tex->GetHeight(),
        m_Tex->GetDC(),
        0, 0,
        m_Tex->GetWidth(), m_Tex->GetHeight(),
        RGB(255, 255, 255)); // 투명색 처리

    //Actor::Render(_dc); // 디버그용 박스
}


