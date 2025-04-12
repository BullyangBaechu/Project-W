#include "pch.h"
#include "Sprite.h"

#include "AssetMgr.h"
#include "Texture.h"

Sprite::Sprite()
	: Asset(ASSET_TYPE::SPRITE)
	, m_Atlas(nullptr)
{
}

Sprite::~Sprite()
{
}

void Sprite::Create(Texture* _Atlas, Vec2 _LeftTop, Vec2 _Slice)
{
	assert(_Atlas);

	m_Atlas = _Atlas;
	m_LeftTop = _LeftTop;
	m_Slice = _Slice;
}


int Sprite::Save(const wstring& _Path)
{
	FILE* pFile = nullptr;

	// write binary
	_wfopen_s(&pFile, _Path.c_str(), L"wb");
	
	// 키, 경로
	fwprintf_s(pFile, L"[KEY]\n");
	fwprintf_s(pFile, GetKey().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[RELATIVE_PATH]\n");
	fwprintf_s(pFile, GetPath().c_str());
	fwprintf_s(pFile, L"\n\n");

	// 대상 텍스쳐
	fwprintf_s(pFile, L"[ATLAS_TEXTURE]\n");
	if (nullptr == m_Atlas)
	{
		fwprintf_s(pFile, L"None");
	}
	else
	{
		fwprintf_s(pFile, L"[KEY]\n%s\n", m_Atlas->GetKey().c_str());
		fwprintf_s(pFile, L"[PATH]\n%s", m_Atlas->GetPath().c_str());
	}
	fwprintf_s(pFile, L"\n\n");


	// 좌상단, 자를 크기, 출력시 위치를 조정할 Offset
	fwprintf_s(pFile, L"[LEFT_TOP]\n%f %f\n\n", m_LeftTop.x, m_LeftTop.y);
	fwprintf_s(pFile, L"[SLICE]\n%f %f\n\n", m_Slice.x, m_Slice.y);
	fwprintf_s(pFile, L"[OFFSET]\n%f %f", m_Offset.x, m_Offset.y);

	fclose(pFile);

	return 0;
}


int Sprite::Load(const wstring& _Path)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _Path.c_str(), L"rb");


	wchar_t szBuff[255] = {};
	while (EOF != fwscanf_s(pFile, L"%s", szBuff, 255))
	{
		if (!wcscmp(L"[KEY]", szBuff))
		{
			wmemset(szBuff, 0, 255);
			fwscanf_s(pFile, L"%s", szBuff, 255);
			SetKey(szBuff);
		}

		else if (!wcscmp(L"[RELATIVE_PATH]", szBuff))
		{
			wmemset(szBuff, 0, 255);
			fwscanf_s(pFile, L"%s", szBuff, 255);
			SetPath(szBuff);
		}

		else if (!wcscmp(L"[ATLAS_TEXTURE]", szBuff))
		{
			wmemset(szBuff, 0, 255);
			fwscanf_s(pFile, L"%s", szBuff, 255);
			if (!wcscmp(L"None", szBuff))
			{
				continue;
			}


			wmemset(szBuff, 0, 255);
			fwscanf_s(pFile, L"%s", szBuff, 255);
			wstring key = szBuff;

			wmemset(szBuff, 0, 255);
			fwscanf_s(pFile, L"%s", szBuff, 255);
			wmemset(szBuff, 0, 255);
			fwscanf_s(pFile, L"%s", szBuff, 255);
			wstring Path = szBuff;

			m_Atlas = AssetMgr::GetInst()->LoadTexture(key, Path);
		}

		else if (!wcscmp(L"[LEFT_TOP]", szBuff))
		{
			fwscanf_s(pFile, L"%f %f", &m_LeftTop.x, &m_LeftTop.y);
		}
		else if (!wcscmp(L"[SLICE]", szBuff))
		{
			fwscanf_s(pFile, L"%f %f", &m_Slice.x, &m_Slice.y);
		}

		else if (!wcscmp(L"[OFFSET]", szBuff))
		{
			fwscanf_s(pFile, L"%f %f", &m_Offset.x, &m_Offset.y);
		}
	}


	fclose(pFile);

	return 0;
}

