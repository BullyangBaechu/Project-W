#include "pch.h"
#include "GameOverLevel.h"
#include "AssetMgr.h"
#include "Engine.h"
#include "LevelMgr.h"
#include "KeyMgr.h"
#include "RunningLevel.h"

GameOverLevel::GameOverLevel()
{
	m_tex = AssetMgr::GetInst()->LoadTexture(L"GameOver", L"Texture\\GameOverFactory_1440x1080.bmp");
}

GameOverLevel::~GameOverLevel()
{
}

void GameOverLevel::Enter()
{
}

void GameOverLevel::Tick()
{
    // 게임 재시작
    if (KEY_TAP(KEY::ENTER))
    {
        ChangeLevel(LEVEL_TYPE::RUNNING);
    }

    // GameStartScene으로 가기
    if (KEY_TAP(KEY::ESC))
    {
        ChangeLevel(LEVEL_TYPE::START);
    }

}

void GameOverLevel::Exit()
{
}

void GameOverLevel::Render(HDC _dc)
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


    // 상단 Game Over 텍스트
    const wchar_t* gameOverText = L"GAME OVER";

    // 큰 폰트 생성 (150pt 크기, Impact)
    HFONT hTitleFont = CreateFont(
        150, 0, 0, 0,
        FW_HEAVY,                // 굵기 (HEAVY는 아주 두꺼움)
        FALSE, FALSE, FALSE,
        HANGUL_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Impact"
    );

    // 폰트 적용
    HFONT hOldFont = (HFONT)SelectObject(_dc, hTitleFont);

    // 텍스트 색상 설정 (진한 빨강)
    SetTextColor(_dc, RGB(220, 30, 30));
    SetBkMode(_dc, TRANSPARENT);

    // 텍스트 크기 측정
    SIZE textSize;
    GetTextExtentPoint32(_dc, gameOverText, wcslen(gameOverText), &textSize);

    // 중앙 X 좌표 계산
    int centerX = (1440 - textSize.cx) / 2;

    // Y 좌표를 상단 중앙으로 
    int posY = 70;

    // 텍스트 출력
    TextOut(_dc, centerX, posY, gameOverText, wcslen(gameOverText));

    // 폰트 원복 및 해제
    SelectObject(_dc, hOldFont);
    ::DeleteObject(hTitleFont);



    // === 하단 안내문구 ===
    const wchar_t* retryText = L"재도전 : Enter";
    const wchar_t* exitText = L"나가기 : ESC";

    HFONT hInfoFont = CreateFont(
        30, 0, 0, 0, 
        FW_HEAVY,
        FALSE, FALSE, FALSE, 
        HANGUL_CHARSET,
        OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, 
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Impact");

    hOldFont = (HFONT)SelectObject(_dc, hInfoFont);

    SetTextColor(_dc, RGB(220, 30, 30));

    SIZE infoSize;
    GetTextExtentPoint32(_dc, retryText, wcslen(retryText), &infoSize);

    Vec2 res = Engine::GetInst()->GetResolution();

    int retryX = (1440 - infoSize.cx) / 2;
    int retryY = (int)(res.y * 0.8f); // 하단 위치

    TextOut(_dc, retryX, retryY, retryText, wcslen(retryText));

    GetTextExtentPoint32(_dc, exitText, wcslen(exitText), &infoSize);

    int exitX = (1440 - infoSize.cx) / 2;
    int exitY = retryY + 40; // 안내문 바로 밑에

    TextOut(_dc, exitX, exitY, exitText, wcslen(exitText));

    SelectObject(_dc, hOldFont);
    ::DeleteObject(hInfoFont);
}


