#include "pch.h"
#include "RunningLevel.h"

#include "Camera.h"
#include "KeyMgr.h"
#include "Force.h"

#include "Engine.h"
#include "Level.h"
#include "Player.h"
#include "Monster.h"
#include "TileActor.h"
#include "Ground.h"
//#include "Spawner.h"
#include "Brick.h"
#include "Bomb.h"
#include "BackGround.h"

#include "TimeMgr.h"
#include "CollisionMgr.h"
#include "AssetMgr.h"
#include "SpawnMgr.h"
#include "RenderMgr.h"
#include "Actor.h"

#include "SimpleBarUI.h"
#include "ProgressBarUI.h"

RunningLevel::RunningLevel()
	: m_GoalX(210000.f)
	, m_IsGameOver(false)
	, m_GameOverTimer(0.f)
{
}

RunningLevel::~RunningLevel()
{
}

void RunningLevel::Enter()
{
	// 재도전 진입
	m_IsGameOver = false;
	m_GameOverTimer = 0.f;
	
	Actor* pActor = nullptr;

	// 카메라 LookAt 설정
	Vec2 vResolution = Engine::GetInst()->GetResolution();
	Camera::GetInst()->SetLookAt(vResolution / 2.f);

	
	// 배경 생성
	pActor = new BackGround;
	pActor->SetName(L"BackGround");
	//pActor->SetPos(Vec2(720.f, 450.f));
	pActor->SetScale(Vec2(1440.f, 900.f));
	pActor->SetCamCheck(false);
	AddObject(ACTOR_TYPE::BACKGROUND, pActor);
	
	
	
	// 진짜 Ground 추가
	pActor = new Ground;
	pActor->SetName(L"Ground");
	pActor->SetPos(Vec2(720.f, GROUND_Y));
	pActor->SetScale(Vec2(1440.f, 96.f));
	pActor->SetCamCheck(true);
	AddObject(ACTOR_TYPE::PLATFORM, pActor);
	 
	

	// 반복 재생을 위한 2번째 Ground
	pActor = new Ground;
	pActor->SetName(L"Ground");
	pActor->SetPos(Vec2(2160.f, GROUND_Y));
	pActor->SetScale(Vec2(1440.f, 96.f));
	pActor->SetCamCheck(true);
	AddObject(ACTOR_TYPE::PLATFORM, pActor);

	// Player 생성
	pActor = new Player;
	pActor->SetName(L"Player");
	pActor->SetPos(Vec2(100.f, 700.f));
	pActor->SetScale(Vec2(120.f, 192.f));
	AddObject(ACTOR_TYPE::PLAYER, pActor);
	RegisterAsPlayer(pActor);

	// SpawnMgr 불러오기
	SpawnMgr::GetInst()->Init();

	// Effect 텍스쳐 & 미리 등록
	AssetMgr::GetInst()->LoadTexture(L"explosion", L"Texture\\explosion_effect_whitebg.bmp");
	AssetMgr::GetInst()->LoadTexture(L"explosion2", L"Texture\\explosion_effect_whitebg_120x120.bmp");
	AssetMgr::GetInst()->LoadTexture(L"guardeffect", L"Texture\\PlayerGuard_120x120.bmp");
	AssetMgr::GetInst()->LoadTexture(L"hiteffect", L"Texture\\HitEffect_50x50_whitebg.bmp");
	AssetMgr::GetInst()->LoadSound(L"bombdestroy", L"Sound\\DroneDestroy.wav");
	AssetMgr::GetInst()->LoadSound(L"brickdestroy", L"Sound\\BrickDestroy.wav");
	AssetMgr::GetInst()->LoadSound(L"hitsound", L"Sound\\PlayerAttack.wav");
	AssetMgr::GetInst()->LoadSound(L"jumpsound", L"Sound\\PlayerJump.wav");



	// 진행도 UI
	m_ProgressBar = new ProgressBarUI;
	m_ProgressBar->SetPos(Vec2(600.f, 20.f));
	AddObject(ACTOR_TYPE::UI, m_ProgressBar);

	SimpleBarUI* backbar = new SimpleBarUI;
	backbar->SetScale(Vec2(400.f, 30.f));
	backbar->SetColor(RGB(100, 100, 100));
	m_ProgressBar->AddChildUI(backbar);

	SimpleBarUI* fillbar = new SimpleBarUI;
	fillbar->SetColor(RGB(0, 255, 0));
	m_ProgressBar->AddChildUI(fillbar);

	m_ProgressBar->SetFillBar(fillbar);




	// 충돌체크
	CollisionMgr::GetInst()->CollisionCheckClear();
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::ENERMY, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::PLATFORM, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::BRICK, true);
	//CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::BOMB, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::BOMB, ACTOR_TYPE::PLAYER, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::SLOWZONE, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLATFORM, ACTOR_TYPE::BRICK, true);
	//CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLATFORM, ACTOR_TYPE::SLOWZONE, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER_PROJECTILE, ACTOR_TYPE::ENERMY, true);

	// 카메라 LookAt 설정
	//Vec2 vResolution = Engine::GetInst()->GetResolution();
	//Camera::GetInst()->SetLookAt(vResolution / 2.f);
}

void RunningLevel::Tick()
{

	// GameOver 체크
	if (m_IsGameOver)
	{
		m_GameOverTimer += DT; // 시간 누적

		if (m_GameOverTimer >= 2.f)
		{
			// 2초 후 GameOverLevel로 전환
			ChangeLevel(LEVEL_TYPE::GAMEOVER);
			return;
		}

		return; // 게임 오버 상태에서는 나머지 로직 멈춤
	}


	Level::Tick();

	// SpawnMgr 로직 시작
	SpawnMgr::GetInst()->Tick();

	// 카메라 자동 이동

	if (m_bFirstFrame)
	{
		m_bFirstFrame = false;
		return;  // 첫 프레임은 카메라 이동하지 않음
	}

	Vec2 cam = Camera::GetInst()->GetLookAt();
	float CamSpeed = Camera::GetInst()->GetCamSpeed();
	cam.x += CamSpeed * DT; 
	Camera::GetInst()->SetLookAt(cam);

	float halfResX = Engine::GetInst()->GetResolution().x / 2.f;
	float groundWidth = 1440.f;

	// Player 진행도 체크
	Actor* pActor = GetPlayer();
	if (pActor)
	{
		float playerX = pActor->GetPos().x;
		float progress = (playerX / m_GoalX) * 100.f;

		if (progress < 1.f)
			progress = 1.f;
		if (progress > 100.f)
			progress = 100.f;

		if (m_ProgressBar)
			m_ProgressBar->SetProgress(progress);
	}


	// GameOver 조건 체크
	//if (m_Player->GetPos().x < -50.f || m_Player->IsDead())
	Vec2 camPos = Camera::GetInst()->GetLookAt();
	Vec2 res = Engine::GetInst()->GetResolution();

	Player* pPlayer = dynamic_cast<Player*>(GetPlayer());
	Vec2 playerPos = pPlayer->GetPos() - camPos + res / 2.f;

	

	if(playerPos.x <-50.f || pPlayer->GetHP() <= 0)
	{
		m_IsGameOver = true;
		m_GameOverTimer = 0.f;
	}

	///////


	if (KEY_TAP(KEY::F5))
	{
		ChangeLevel(LEVEL_TYPE::TEST);
	}

	if (KEY_TAP(KEY::ESC))
	{
		ChangeLevel(LEVEL_TYPE::START);
	}
}

void RunningLevel::Exit()
{
	DeleteAllObject();
}

void RunningLevel::Render(HDC _dc)
{

	Level::Render(_dc);

	if (m_IsGameOver)
	{
		// GameOver 텍스트 띄우기
		const wchar_t* gameOverText = L"YOU DIED";

		HFONT hFont = CreateFont(
			150,      // 폰트 높이 (크기) 
			0,        // 너비 (0이면 자동 비율 유지)
			0, 0,     // 기울기, 회전 각도
			FW_HEAVY, // 굵기: FW_BOLD(굵음), FW_HEAVY(매우 굵음), FW_NORMAL(보통)
			FALSE,    // 이탤릭 여부
			FALSE,    // 밑줄 여부
			FALSE,    // 취소선 여부
			HANGUL_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			L"Optimus Princeps"  // 폰트 이름 
		);

		HFONT hOldFont = (HFONT)SelectObject(_dc, hFont);

		// 폰트 컬러
		SetTextColor(_dc, RGB(170, 30, 30));
		SetBkMode(_dc, TRANSPARENT);

		SIZE textSize;
		GetTextExtentPoint32(_dc, gameOverText, wcslen(gameOverText), &textSize);

		int centerX = (1440 - textSize.cx) / 2;
		// Y축 원하는 값으로 OffSet 설정
		int offsetY = -200;
		int centerY = (1080 - textSize.cy) / 2 + offsetY;

		TextOut(_dc, centerX, centerY, gameOverText, wcslen(gameOverText));

		SelectObject(_dc, hOldFont);
		::DeleteObject(hFont);

	}

}

