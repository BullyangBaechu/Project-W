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


RunningLevel::RunningLevel()
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

	/*
	// 배경 생성
	pActor = new BackGround;
	pActor->SetName(L"BackGround");
	//pActor->SetPos(Vec2(720.f, 450.f));
	pActor->SetScale(Vec2(1440.f, 900.f));
	pActor->SetCamCheck(false);
	AddObject(ACTOR_TYPE::BACKGROUND, pActor);
	*/
	

	m_BGTex = AssetMgr::GetInst()->LoadTexture(L"BG", L"Texture\\BackGroundFactory.bmp");

	// Player 생성
	pActor = new Player;
	pActor->SetName(L"Player");
	pActor->SetPos(Vec2(100.f, 700.f));
	pActor->SetScale(Vec2(100, 100));
	AddObject(ACTOR_TYPE::PLAYER, pActor);
	RegisterAsPlayer(pActor);

	// 예비 Ground 오브젝트 추가
	//pActor = new Ground;
	//pActor->SetPos(Vec2(640.f, GROUND_Y + 50));
	////pActor->SetScale(Vec2(10000, 100));
	//AddObject(ACTOR_TYPE::PLATFORM, pActor);

	
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


	// SpawnMgr 불러오기
	SpawnMgr::GetInst()->Init();

	CollisionMgr::GetInst()->CollisionCheckClear();
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::ENERMY, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::PLATFORM, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::BRICK, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::BOMB, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLATFORM, ACTOR_TYPE::BRICK, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER_PROJECTILE, ACTOR_TYPE::ENERMY, true);

	// 카메라 LookAt 설정
	//Vec2 vResolution = Engine::GetInst()->GetResolution();
	//Camera::GetInst()->SetLookAt(vResolution / 2.f);
}

void RunningLevel::Tick()
{
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
