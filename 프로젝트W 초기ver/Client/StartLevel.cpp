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

#include "CollisionMgr.h"
#include "Camera.h"

StartLevel::StartLevel()
{
}

StartLevel::~StartLevel()
{
}

void StartLevel::Enter()
{
	
}

void StartLevel::Tick()
{
	Level::Tick();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeLevel(LEVEL_TYPE::RUNNING);
	}

	if (KEY_TAP(KEY::F4))
	{
		ChangeLevel(LEVEL_TYPE::TEST);
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

void StartLevel::Exit()
{
	DeleteAllObject();
}


