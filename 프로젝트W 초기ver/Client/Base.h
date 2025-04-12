#pragma once

// 엔진 최상위 기본(부모) 클래스
class Base
{
private:
	static UINT		m_GlobalID;
	const  UINT		m_ID;
	wstring			m_Name;

public:
	UINT GetID() { return m_ID; }

	void SetName(const wstring& _Name) { m_Name = _Name; }
	const wstring& GetName() { return m_Name; }

public:
	Base();
	Base(const Base& _Other);
	virtual ~Base();
};

