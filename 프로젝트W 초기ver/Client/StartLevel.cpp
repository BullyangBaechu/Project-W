#include "pch.h"
#include "StartLevel.h"

#include "Camera.h"
#include "KeyMgr.h"
#include "Force.h"

#include "Engine.h"
#include "Level.h"
#include "Player.h"
#include "Monster.h"
#include "TileActor.h"
#include "Ground.h"

#include "AssetMgr.h"
#include "CollisionMgr.h"
#include "Camera.h"
#include "Texture.h"
#include "Sound.h"

StartLevel::StartLevel()
{
	m_tex = AssetMgr::GetInst()->LoadTexture(L"GameStart", L"Texture\\GameStart_1440x1080.bmp");
}

StartLevel::~StartLevel()
{
}

void StartLevel::Enter()
{
	m_bgm = AssetMgr::GetInst()->LoadSound(L"StartBGM", L"Sound\\StartBGM.wav");
	m_bgm->PlayToBGM(true);
}

void StartLevel::Tick()
{
	Level::Tick();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeLevel(LEVEL_TYPE::RUNNING);
	}

	if (KEY_TAP(KEY::F5))
	{
		ChangeLevel(LEVEL_TYPE::TEST);
	}
	
}

void StartLevel::Exit()
{
	DeleteAllObject();
}

void StartLevel::Render(HDC _dc)
{
	if (m_tex)
	{
		int texW = m_tex->GetWidth();
		int texH = m_tex->GetHeight();

		TransparentBlt(_dc,
			0, 0, texW, texH,     // 출력 위치와 크기
			m_tex->GetDC(),
			0, 0, texW, texH,     // 원본 위치와 크기
			RGB(255, 0, 255));
	}

	// ===== 하단 중앙에 안내 문구 출력 =====
	const wchar_t* infoText = L"Press ENTER to Play Game";

	HFONT hInfoFont = CreateFont(
		40, 0, 0, 0,
		FW_BOLD,
		FALSE, FALSE, FALSE,
		HANGUL_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		L"Impact" 
	);

	HFONT hOldFont = (HFONT)SelectObject(_dc, hInfoFont);

	SetTextColor(_dc, RGB(0, 0, 0));  
	SetBkMode(_dc, TRANSPARENT);

	SIZE textSize;
	GetTextExtentPoint32(_dc, infoText, wcslen(infoText), &textSize);

	Vec2 res = Engine::GetInst()->GetResolution();

	int textX = (res.x - textSize.cx) / 2;
	int textY = (int)(res.y * 0.85f);  // 하단 85% 위치

	TextOut(_dc, textX, textY, infoText, wcslen(infoText));

	SelectObject(_dc, hOldFont);
	::DeleteObject(hInfoFont);
}


