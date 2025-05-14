#include "pch.h"
#include "ProgressBarUI.h"

ProgressBarUI::ProgressBarUI()
	: m_Progress(0.f)
	, m_FillBar(nullptr)
{
}

ProgressBarUI::~ProgressBarUI()
{
}

void ProgressBarUI::Tick_UI()
{
	if (m_FillBar)
	{
		float maxWidth = 400.f;
		m_FillBar->SetScale(Vec2(maxWidth * (m_Progress / 100.f), 30.f));
	}
}

void ProgressBarUI::Render_UI(HDC _dc)
{
}
