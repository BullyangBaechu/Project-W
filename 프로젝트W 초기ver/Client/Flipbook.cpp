#include "pch.h"
#include "Flipbook.h"

#include "AssetMgr.h"
#include "Sprite.h"

Flipbook::Flipbook()
	: Asset(ASSET_TYPE::FLIPBOOK)
{
}

Flipbook::~Flipbook()
{
}

void Flipbook::AddSprite(Sprite* _Sprite, int _Idx)
{
	if (-1 == _Idx)
	{
		m_Sprites.push_back(_Sprite);
	}
	else
	{
		if (m_Sprites.size() <= _Idx)
		{
			m_Sprites.resize(_Idx + 1);
		}

		m_Sprites[_Idx] = _Sprite;
	}
}

int Flipbook::Save(const wstring& _Path)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _Path.c_str(), L"wb");

	// 이름, 키, 경로
	fwprintf_s(pFile, L"[KEY]\n");
	fwprintf_s(pFile, GetKey().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[RELATIVE_PATH]\n");
	if (GetPath().empty())
		fwprintf_s(pFile, L"None");
	else
		fwprintf_s(pFile, GetPath().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[SPRITE_INFO]\n");	

	for (size_t i = 0; i < m_Sprites.size(); ++i)
	{
		fwprintf_s(pFile, L"[KEY]\t\t%s\n", m_Sprites[i]->GetKey().c_str());
		fwprintf_s(pFile, L"[PATH]\t%s\n\n", m_Sprites[i]->GetPath().c_str());
	}

	fclose(pFile);

	return S_OK;
}

int Flipbook::Load(const wstring& _Path)
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

			if (wcscmp(L"None", szBuff))
			{
				SetPath(szBuff);
			}
		}
		else if (!wcscmp(L"[SPRITE_INFO]", szBuff))
		{
			wmemset(szBuff, 0, 255);
			wstring key, path;
			while (EOF != fwscanf_s(pFile, L"%s", szBuff, 255))
			{
				if (!wcscmp(L"[KEY]", szBuff))
				{
					wmemset(szBuff, 0, 255);
					fwscanf_s(pFile, L"%s", szBuff, 255);
					key = szBuff;
				}

				else if (!wcscmp(L"[PATH]", szBuff))
				{
					wmemset(szBuff, 0, 255);
					fwscanf_s(pFile, L"%s", szBuff, 255);
					path = szBuff;

					Sprite* pSprite = AssetMgr::GetInst()->LoadSprite(key, path);
					AddSprite(pSprite);
				}
			}
		}
	}

	fclose(pFile);

	return S_OK;
}
