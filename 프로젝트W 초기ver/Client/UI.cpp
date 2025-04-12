#include "pch.h"
#include "UI.h"

#include "KeyMgr.h"
#include "AssetMgr.h"
#include "Texture.h"



UI::UI()
	: m_LBtnDowned(false)
	, m_MouseHover(false)
	, m_PrevMouseHover(false)
	, m_ParentUI(nullptr)
{
	SetCamCheck(false);
}

UI::~UI()
{
	Delete_Vec(m_vecChildUI);
}

void UI::Tick()
{
	Vec2 vPos = GetPos();
	m_FinalPos = vPos;

	if (m_ParentUI)
	{
		m_FinalPos += m_ParentUI->m_FinalPos;
	}

	// 마우스 왼클릭 뗄 때 눌림상태 해제하기
	if (m_LBtnDowned && KEY_RELEASED(KEY::LBTN))
	{
		m_LBtnDowned = false;
	}

	// DebugRender 
	DrawDebugRect(m_FinalPos + GetScale() / 2.f, GetScale(), PEN_TYPE::GREEN, 0.f);


	// UI 가 할 일
	Tick_UI();


	// 자식 UI Tick 호출
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Tick();
	}
}

void UI::Render(HDC _DC)
{
	Render_UI(_DC);

	// 자식 UI Tick 호출
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Render(_DC);
	}
}

void UI::MouseCheck()
{
	Vec2 vMousePos = KeyMgr::GetInst()->GetMousePos();

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	m_PrevMouseHover = m_MouseHover;

	if (vPos.x <= vMousePos.x && vMousePos.x <= vPos.x + vScale.x
		&& vPos.y <= vMousePos.y && vMousePos.y <= vPos.y + vScale.y)
	{
		m_MouseHover = true;
	}
	else
	{
		m_MouseHover = false;
	}
}