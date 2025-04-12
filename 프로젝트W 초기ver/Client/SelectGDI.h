#pragma once


class SelectGDI
{
private:
	HDC			m_DC;
	HGDIOBJ		m_PrevObj;

public:
	SelectGDI(HDC _DC, HGDIOBJ _Obj);
	~SelectGDI();
};

