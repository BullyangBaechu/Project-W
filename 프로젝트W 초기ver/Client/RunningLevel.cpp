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
#include "TimeMgr.h"

#include "CollisionMgr.h"


RunningLevel::RunningLevel()
{
}

RunningLevel::~RunningLevel()
{
}

void RunningLevel::Enter()
{
	Actor* pActor = nullptr;

	// Player 생성
	pActor = new Player;
	pActor->SetName(L"Player");
	pActor->SetPos(Vec2(640.f, 400.f));
	pActor->SetScale(Vec2(100, 100));
	AddObject(ACTOR_TYPE::PLAYER, pActor);
	RegisterAsPlayer(pActor);

	// Ground 오브젝트 추가
	pActor = new Ground;
	pActor->SetPos(Vec2(640.f, GROUND_Y));
	//pActor->SetScale(Vec2(10000, 100));
	
	AddObject(ACTOR_TYPE::PLATFORM, pActor);

	CollisionMgr::GetInst()->CollisionCheckClear();
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::ENERMY, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::PLATFORM, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER_PROJECTILE, ACTOR_TYPE::ENERMY, true);

	// 카메라 LookAt 설정
	Vec2 vResolution = Engine::GetInst()->GetResolution();
	Camera::GetInst()->SetLookAt(vResolution / 2.f);
}

void RunningLevel::Tick()
{
	Level::Tick();

	Vec2 cam = Camera::GetInst()->GetLookAt();
	cam.x += 300.f * DT; // 초당 300 속도
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

