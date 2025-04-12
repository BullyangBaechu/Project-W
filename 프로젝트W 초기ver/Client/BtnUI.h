#pragma once
#include "UI.h"


class BtnUI :
    public UI
{
private:
    Texture*     m_BaseImage;
    Texture*     m_HoverImage;
    Texture*     m_PressedImage;

    Texture*     m_CurImage;

    // 함수포인터
    // CallBack
    UI_CALL_BACK m_CallBackFunc;

public:
    void SetBaseImage(Texture* _Tex);
    void SetHoverImage(Texture* _Tex) { m_HoverImage = _Tex; }
    void SetPressedImage(Texture* _Tex) { m_PressedImage = _Tex; }
    void SetCallBack(UI_CALL_BACK _Func) { m_CallBackFunc = _Func; }

public:
    void Tick_UI() override;
    void Render_UI(HDC _dc) override;

    virtual void BeginHover() override;
    virtual void EndHover() override;

    virtual void LbtnDown() {} 
    virtual void LbtnUP() {}    
    virtual void LbtnClicked(); 

public:
    BtnUI();
    ~BtnUI();
};