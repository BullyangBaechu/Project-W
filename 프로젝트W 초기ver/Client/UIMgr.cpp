#include "pch.h"
#include "UIMgr.h"

#include "KeyMgr.h"

#include "LevelMgr.h"
#include "Level.h"
#include "UI.h"

UIMgr::UIMgr()
	: m_FocusedUI(nullptr)
	, m_PriorityUI(nullptr)
{
}

UIMgr::~UIMgr()
{
}

void UIMgr::Tick()
{
	Level* pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;
	
	vector<Actor*>& vecUI = pCurLevel->m_Actor[(UINT)ACTOR_TYPE::UI];
	vector<Actor*>::reverse_iterator riter = vecUI.rbegin();

	for (riter; riter != vecUI.rend(); ++riter)
	{
		UI* pUI = dynamic_cast<UI*>(*riter);
		assert(pUI);
				
		// 부모 UI 부터 자식UI 순서로 Level 순회를 한다.
		m_PriorityUI = FindPriorityUI(pUI);

		if (m_PriorityUI)
		{
			ClickCheck(m_PriorityUI);

			// reverse_iterator 가 가리키는 대상을 컨테이너에서 제거해야 하는 경우
			vecUI.erase((riter + 1).base());
			vecUI.push_back(pUI);

			break;
		}
	}
}

UI* UIMgr::FindPriorityUI(UI* _ParentUI)
{	
	static list<UI*> queue;
	queue.clear();
	queue.push_back(_ParentUI);

	UI* pPriorityUI = nullptr;

	while (!queue.empty())
	{
		UI* pTargetUI = queue.front();
		queue.pop_front();
		const vector<UI*>& vecChild = pTargetUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

		// 마우스 호버 체크
		HoverCheck(pTargetUI);	

		// UI 우선순위 체크
		if (pTargetUI->m_MouseHover && (KEY_TAP(KEY::LBTN) || KEY_RELEASED(KEY::LBTN)))
		{
			pPriorityUI = pTargetUI;
		}
	}

	return pPriorityUI;
}

void UIMgr::HoverCheck(UI* _UI)
{
	_UI->MouseCheck();

	if (false == _UI->m_PrevMouseHover && _UI->m_MouseHover)
		_UI->BeginHover();
	else if (_UI->m_MouseHover)
		_UI->OnHover();
	else if (_UI->m_PrevMouseHover && false == _UI->m_MouseHover)
		_UI->EndHover();
}

void UIMgr::ClickCheck(UI* _UI)
{
	if (KEY_TAP(KEY::LBTN))
	{
		_UI->LBtnDown_Mgr();
	}

	// UI 왼쪽 클릭
	if (KEY_RELEASED(KEY::LBTN) && _UI->m_LBtnDowned)
	{
		_UI->LbtnClicked();
	}
}
