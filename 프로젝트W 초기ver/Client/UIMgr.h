#pragma once

class UI;

class UIMgr
{
	SINGLE(UIMgr);
private:
	UI*		m_FocusedUI;	// 포커싱 중인 최상위 부모 UI
	UI*		m_PriorityUI;	// 우선순위 UI

public:
	void Tick();

private:
	UI* FindPriorityUI(UI* _ParentUI);
	void HoverCheck(UI* _UI);
	void ClickCheck(UI* _UI);

};

