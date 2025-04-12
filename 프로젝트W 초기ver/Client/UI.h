#pragma once
#include "Actor.h"

class Texture;

class UI :
    public Actor
{
public:
    typedef void(*UI_CALL_BACK)(void);

private:
    Vec2        m_FinalPos;   
    bool        m_LBtnDowned;

    bool        m_MouseHover;
    bool        m_PrevMouseHover;

    UI*         m_ParentUI;
    vector<UI*> m_vecChildUI;

public:
    Vec2 GetFinalPos() { return m_FinalPos; }
    void AddChildUI(UI* _ChildUI)
    {
        m_vecChildUI.push_back(_ChildUI);
        _ChildUI->m_ParentUI = this;
    }
    const vector<UI*>& GetChildUI() { return m_vecChildUI; }
    bool IsLBtnDown() { return m_LBtnDowned; }

public:
    virtual void Tick() final;
    virtual void Tick_UI() = 0;

    virtual void Render(HDC _DC) final;
    virtual void Render_UI(HDC _dc) = 0;
         

private:
    void LBtnDown_Mgr()
    { 
        m_LBtnDowned = true; 
        LbtnDown();
    }

    virtual void BeginHover() {}    // 마우스가 UI 위로 올라온 순간
    virtual void OnHover() {}       // 마우스가 UI 위에 있을 때
    virtual void EndHover() {}      // 마우스가 UI 영역을 벗어날 때

    virtual void LbtnDown() {}      // UI 위에서 마우스 왼쪽버튼으로 누른 경우
    virtual void LbtnUP() {}        // UI 위에서 마우스 왼쪽버튼을 뗄 경우
    virtual void LbtnClicked() {}   // UI 를 클릭한 경우

    virtual void MouseCheck();

public:
    UI();
    ~UI();

    friend class UIMgr;
};

