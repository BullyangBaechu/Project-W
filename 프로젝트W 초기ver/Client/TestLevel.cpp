#include "pch.h"
#include "TestLevel.h"

#include "Camera.h"
#include "KeyMgr.h"
#include "Force.h"

#include "Engine.h"
#include "Level.h"
#include "Player.h"
#include "Monster.h"
#include "TileActor.h"
#include "Ground.h"

#include "CollisionMgr.h"



TestLevel::TestLevel()
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Enter()
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
	pActor->SetPos(Vec2(640.f, 700.f));
	pActor->SetScale(Vec2(100, 100));
	AddObject(ACTOR_TYPE::PLATFORM, pActor);

	// Monster 생성
	pActor = new Monster;
	pActor->SetName(L"Monster");
	pActor->SetPos(Vec2(800, 100));
	pActor->SetScale(Vec2(120, 120));
	((Monster*)pActor)->SetInitPos(pActor->GetPos());
	AddObject(ACTOR_TYPE::ENERMY, pActor);

	// Tile 액터 생성
	pActor = new TileActor;
	pActor->SetName(L"TileActor");
	pActor->SetPos(Vec2(200.f, 200.f));
	pActor->SetScale(Vec2(64.f * 5.f, 64.f * 5.f));
	((TileActor*)pActor)->ShowGrid(false);

	AddObject(ACTOR_TYPE::TILE, pActor);

	CollisionMgr::GetInst()->CollisionCheckClear();
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::ENERMY, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER, ACTOR_TYPE::PLATFORM, true);
	CollisionMgr::GetInst()->CollisionCheck(ACTOR_TYPE::PLAYER_PROJECTILE, ACTOR_TYPE::ENERMY, true);

	// 카메라 LookAt 설정
	Vec2 vResolution = Engine::GetInst()->GetResolution();
	Camera::GetInst()->SetLookAt(vResolution / 2.f);
}

void TestLevel::Tick()
{
	Level::Tick();

	if (KEY_TAP(KEY::F5))
	{
		ChangeLevel(LEVEL_TYPE::RUNNING);
	}

	if (KEY_TAP(KEY::F4))
	{
		ChangeLevel(LEVEL_TYPE::START);
	}

	// 마우스 왼클릭이 발생하면, 해당위치에 Force 객체를 생성시킴
	if (KEY_TAP(KEY::LBTN))
	{
		Force* pForce = new Force;
		// 마우스를 클릭한 지점의 실제 좌표를 계산해서 배치
		pForce->SetPos(Camera::GetInst()->GetRealPos(KeyMgr::GetInst()->GetMousePos()));
		pForce->SetDuration(0.05f);
		pForce->SetForceScale(1000000.f);
		pForce->SetRange(300.f);
		pForce->SetActorType(ACTOR_TYPE::PLAYER, true);
		pForce->SetActorType(ACTOR_TYPE::ENERMY, true);
		CreateActor(ACTOR_TYPE::FORCE, pForce);
	}
}

void TestLevel::Exit()
{
	DeleteAllObject();
}

