#include "pch.h"
#include "SimpleBarUI.h"
#include "RenderMgr.h"


void SimpleBarUI::Render_UI(HDC _dc)
{
    Vec2 pos = GetFinalPos();
    Vec2 scale = GetScale();

    HBRUSH hBrush = CreateSolidBrush(m_color);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);

    Rectangle(_dc, (int)pos.x, (int)pos.y, (int)(pos.x + scale.x), (int)(pos.y + scale.y));

    SelectObject(_dc, hOldBrush);
    DeleteObject(hBrush);
}
