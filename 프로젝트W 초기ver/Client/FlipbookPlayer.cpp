#include "pch.h"
#include "FlipbookPlayer.h"

#include "TimeMgr.h"

#include "Actor.h"
#include "Flipbook.h"
#include "Sprite.h"
#include "Texture.h"

FlipbookPlayer::FlipbookPlayer()
	: Component(COMPONENT_TYPE::FLIPBOOK_PLAYER)
	, m_CurFlipbook(nullptr)
	, m_CurSprite(0)
	, m_FPS(30)
	, m_RepeatCount(1)
	, m_PlayCount(0)
	, m_Finish(false)
	, m_AccTime(0.f)
{
}

FlipbookPlayer::~FlipbookPlayer()
{
}

void FlipbookPlayer::FinalTick()
{
	if (nullptr == m_CurFlipbook)
		return;

	

	if (m_Finish)
	{
		if (0 == m_RepeatCount)
		{
			m_Finish = false;
			m_CurSprite = 0;
		}
		else if (m_PlayCount < m_RepeatCount)
		{
			m_Finish = false;
			m_CurSprite = 0;
		}		
	}

	if (m_Finish)
		return;

	// °è»ê
	float LimitTime = 1.f / m_FPS;
	m_AccTime += DT;

	if (LimitTime <= m_AccTime)
	{
		m_AccTime -= LimitTime;

		if (m_CurSprite < m_CurFlipbook->GetMaxSpriteCount() - 1)
			++m_CurSprite;
		else
		{
			m_Finish = true;
			++m_PlayCount;
		}		
	}

}

void FlipbookPlayer::Render(HDC _dc)
{
	if (nullptr == m_CurFlipbook)
		return;

	// ·»´õ¸µ
	Sprite* pCurSprite = m_CurFlipbook->GetSprite(m_CurSprite);
		
	Vec2 vSlice = pCurSprite->GetSlice();
	Vec2 vOffset = pCurSprite->GetOffset();
	Vec2 vOwnerPos = GetOwner()->GetRenderPos();

	TransparentBlt(_dc
				, int(vOwnerPos.x - vSlice.x / 2.f + vOffset.x)
				, int(vOwnerPos.y - vSlice.y / 2.f + vOffset.y)
				, int(vSlice.x)
				, int(vSlice.y)
				, pCurSprite->GetAltas()->GetDC()
				, int(pCurSprite->GetLeftTop().x)
				, int(pCurSprite->GetLeftTop().y)
				, int(vSlice.x)
				, int(vSlice.y)
				, RGB(255, 0, 255));
}

void FlipbookPlayer::AddFlipbook(Flipbook* _Flipbook, int _Index)
{
	if (-1 == _Index)
	{
		m_vecFlipbook.push_back(_Flipbook);
	}
	else
	{
		if (m_vecFlipbook.size() <= _Index)
		{
			m_vecFlipbook.resize(_Index + 1);
		}
		m_vecFlipbook[_Index] = _Flipbook;
	}
}

void FlipbookPlayer::Play(int _Idx, float _FPS, int _StartSpriteIdx, UINT _Repeat)
{
	m_FPS = _FPS;
	m_RepeatCount = _Repeat;
	m_PlayCount = 0;
	m_Finish = false;

	m_CurFlipbook = m_vecFlipbook[_Idx];
	m_CurSprite = _StartSpriteIdx;

	m_AccTime = 0.f;
}

