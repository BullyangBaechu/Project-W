#include "pch.h"
#include "PanelUI.h"

#include "KeyMgr.h"

PanelUI::PanelUI()
{
}

PanelUI::~PanelUI()
{
}

void PanelUI::Tick_UI()
{
	if (IsLBtnDown())
	{
		Vec2 vMousePos = KeyMgr::GetInst()->GetMousePos();

		Vec2 vDiff = vMousePos - m_LBtnPressedPos;

		SetPos(GetPos() + vDiff);

		m_LBtnPressedPos = vMousePos;
	}	
}

void PanelUI::Render_UI(HDC _dc)
{

}

void PanelUI::BeginHover()
{
}

void PanelUI::EndHover()
{
}

void PanelUI::LbtnDown()
{
	m_LBtnPressedPos = KeyMgr::GetInst()->GetMousePos();
}

void PanelUI::LbtnClicked()
{
}

