#pragma once

#include "Base.h"

// 공유해서 사용하는 자원
class Asset
	: public Base
{
private:
	wstring				m_Key;	// 해당 에셋을 검색할때 사용할 키값
	wstring				m_Path;	// 생성된(로딩된) 에셋이 파일 어디에서 로딩된 에셋인지 경로를 기록(에셋의 출처)
	const ASSET_TYPE	m_Type; // 어떤 타입의 에셋인지 확인하는 용도

protected:
	void SetKey(const wstring& _Key) { m_Key = _Key; }
	void SetPath(const wstring& _Path) { m_Path = _Path; }

public:
	const wstring& GetKey() { return m_Key; }
	const wstring& GetPath() { return m_Path; }

	// RTTI(Runtime Type Information(Identification))
	// C++ 문법으로 제공되는 RTTI 는 dynamic_cast
	ASSET_TYPE GetType() { return m_Type; }

public:
	Asset(ASSET_TYPE _Type);
	~Asset();

	friend class AssetMgr;
};

