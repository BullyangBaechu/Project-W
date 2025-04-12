#include "pch.h"
#include "SelectGDI.h"

SelectGDI::SelectGDI(HDC _DC, HGDIOBJ _Obj)
	: m_DC(_DC)
	, m_PrevObj(nullptr)
{
	m_PrevObj = SelectObject(m_DC, _Obj);
}

SelectGDI::~SelectGDI()
{
	SelectObject(m_DC, m_PrevObj);
}
