#include "pch.h"
#include "BtnUI.h"

#include "KeyMgr.h"
#include "Texture.h"


BtnUI::BtnUI()
	: m_BaseImage(nullptr)
	, m_HoverImage(nullptr)
	, m_PressedImage(nullptr)	
	, m_CurImage(nullptr)
	, m_CallBackFunc(nullptr)
{

}

BtnUI::~BtnUI()
{
}


void BtnUI::Tick_UI()
{

}

void BtnUI::Render_UI(HDC _dc)
{
	Vec2 vPos = GetFinalPos();

	BLENDFUNCTION blend = {};
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;

	int width = m_CurImage->GetWidth();
	int height = m_CurImage->GetHeight();

	AlphaBlend(_dc, int(vPos.x), int(vPos.y), width, height
		, m_CurImage->GetDC()
		, 0, 0, (int)width, (int)height, blend);
}

void BtnUI::SetBaseImage(Texture* _Tex)
{
	m_CurImage = m_BaseImage = _Tex;
	SetScale(Vec2(m_CurImage->GetWidth(), m_CurImage->GetHeight()));
}

void BtnUI::BeginHover()
{
	if(nullptr != m_HoverImage)
		m_CurImage = m_HoverImage;
}

void BtnUI::EndHover()
{
	m_CurImage = m_BaseImage;
}

void BtnUI::LbtnClicked()
{
	if (nullptr != m_CallBackFunc)
	{
		m_CallBackFunc();
		KeyMgr::GetInst()->SetLock(1.f);
	}	
}