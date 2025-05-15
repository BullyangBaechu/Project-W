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

StartLevel::StartLevel()
{
	m_tex = AssetMgr::GetInst()->LoadTexture(L"GameOver", L"Texture\\GameOverFactory_1440x1080.bmp");
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

}


