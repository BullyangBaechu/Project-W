#include "pch.h"
#include "EscapeObject.h"
#include "AssetMgr.h"
#include "Camera.h"
#include "Texture.h"


EscapeObject::EscapeObject()
{
	SetName(L"EscapeObject");

	m_tex = AssetMgr::GetInst()->LoadTexture(L"EscapeObject", L"Texture\\Flag_200x400.bmp");
}

EscapeObject::~EscapeObject()
{
}

void EscapeObject::Tick()
{
}

void EscapeObject::Render(HDC _dc)
{
    if (m_tex)
    {
        Vec2 renderPos = GetRenderPos();
        float w = m_tex->GetWidth();
        float h = m_tex->GetHeight();

        TransparentBlt(_dc,
            (int)(renderPos.x - w / 2.f),
            (int)(renderPos.y - h / 2.f),
            (int)w,
            (int)h,
            m_tex->GetDC(),
            0, 0,
            (int)w,
            (int)h,
            RGB(0, 0, 0)); // 여백 투명
    }
}
