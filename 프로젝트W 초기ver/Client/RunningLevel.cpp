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
	
	

	//m_BGTex = AssetMgr::GetInst()->LoadTexture(L"BG", L"Texture\\BackGroundFactory.bmp");

	


	
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

	// Effect 텍스쳐 미리 등록
	AssetMgr::GetInst()->LoadTexture(L"explosion", L"Texture\\explosion_effect_whitebg.bmp");
	AssetMgr::GetInst()->LoadTexture(L"explosion2", L"Texture\\explosion_effect_whitebg_120x120.bmp");
	AssetMgr::GetInst()->LoadTexture(L"guardeffect", L"Texture\\PlayerGuard_120x120.bmp");
	

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

// 세그먼트 구조 완성 시 삭제될 듯? 
// 배경을 그리기 위해 추가한 함수인데, 세그먼트 구조로 가면 배경도 Actor로 만들 듯 함
// 핵심 : 카메라 중심으로 텍스쳐를 좌우로 여러 장 생성

/*
void RunningLevel::Render(HDC _dc)
{
	// 1. 기본 정보
	Vec2 camPos = Camera::GetInst()->GetLookAt();
	Vec2 res = Engine::GetInst()->GetResolution();


	float texWidth = m_BGTex->GetWidth();
	float texHeight = m_BGTex->GetHeight();
	

	// 화면 대비 스케일 계산
	float scaleX = res.x / texWidth;
	float scaleY = res.y / texHeight;

	// 전체 화면에 맞춰 scaling 하기 -> 일단 프레임 드랍 심해서 포기

	// 전체 배경 비율 유지 or 꽉 채우기 (선택)
	// float scale = min(scaleX, scaleY);  // 비율 유지
	// float scaledWidth = texWidth * scale;
	// float scaledHeight = texHeight * scale;

	// "꽉 채우기" 원하니까 각각 따로 스케일 적용
	//float scaledWidth = texWidth * scaleX;
	//float scaledHeight = texHeight * scaleY;

	float halfResX = res.x / 2.f;

	// 2. 카메라 기준 반복 출력 범위 계산
	int start = (int)((camPos.x - halfResX) / texWidth) - 1;
	int end = (int)((camPos.x + halfResX) / texWidth) + 1;

	// 3. 반복 렌더링
	for (int i = start; i <= end; ++i)
	{
		float worldX = i * texWidth + texWidth / 2.f;
		Vec2 worldPos(worldX, texHeight / 2.f);
		Vec2 renderPos = Camera::GetInst()->GetRenderPos(worldPos);

		TransparentBlt(_dc,
			(int)(renderPos.x - texWidth / 2.f),
			(int)(renderPos.y - texHeight / 2.f),
			(int)texWidth,
			(int)texHeight,
			m_BGTex->GetDC(),
			0, 0,
			(int)texWidth,
			(int)texHeight,
			RGB(255, 0, 255)); // 마젠타 배경 투명 처리
	}

	// 4. 기존 Actor들 렌더링
	Level::Render(_dc);
}
*/
