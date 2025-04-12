#include "pch.h"
#include "EditorLevel.h"

#include "Sound.h"

#include "PathMgr.h"
#include "AssetMgr.h"
#include "Sprite.h"
#include "LevelMgr.h"

#include "TileActor.h"
#include "TileMap.h"

#include "Engine.h"
#include "KeyMgr.h"
#include "resource.h"

#include "Camera.h"

#include "UI.h"
#include "BtnUI.h"
#include "PanelUI.h"

EditorLevel::EditorLevel()
	: m_hMenu(nullptr)
    , m_TileActor(nullptr)
    , m_CurSprite(nullptr)
{
	m_hMenu = LoadMenu(Engine::GetInst()->GetMainInst(), MAKEINTRESOURCE(IDC_CLIENT));
}

EditorLevel::~EditorLevel()
{
	DestroyMenu(m_hMenu);
}

TileActor* EditorLevel::GetTileActor()
{
    if (nullptr == m_TileActor)
    {
        m_TileActor = new TileActor;
        AddObject(ACTOR_TYPE::TILE, m_TileActor);
    }
    return m_TileActor;
}

void EditorLevel::Enter()
{
    // 메인 윈도우에 메뉴바 추가	
    SetMenu(Engine::GetInst()->GetMainWnd(), m_hMenu);

    // 메뉴바가 붙은 기준으로 비트맵 해상도를 유지하기위한 윈도우 크기 재계산
    Vec2 vResol = Engine::GetInst()->GetResolution();
    Engine::GetInst()->ChangeWindowResolution((UINT)vResol.x, (UINT)vResol.y);

    // 카메라 위치 초기화
    Camera::GetInst()->SetLookAt(Engine::GetInst()->GetResolution() / 2.f);

    // Editor 용 UI 생성
    CreateEditorUI();

    // 배경음 재생
    Sound* pSound = AssetMgr::GetInst()->LoadSound(L"BGM_01", L"Sound\\DM.wav");
    assert(pSound);
    pSound->SetVolume(50.f);
    pSound->PlayToBGM(true);
}


void EditorLevel::Exit()
{
    // 메인 윈도우에 메뉴바 제거
    SetMenu(Engine::GetInst()->GetMainWnd(), nullptr);

    // 메뉴바가 떨어진 기준으로 비트맵 해상도를 유지하기위한 윈도우 크기 재계산
    Vec2 vResol = Engine::GetInst()->GetResolution();
    Engine::GetInst()->ChangeWindowResolution((UINT)vResol.x, (UINT)vResol.y);
}


void EditorLevel::Tick()
{
    Level::Tick();	

    if (KEY_TAP(KEY::F4))
    {
        ChangeLevel(LEVEL_TYPE::START);
    }

    ChangeSprite();

    TileClick();
}


void EditorLevel::TileClick()
{
    if (!KEY_RELEASED(KEY::LBTN))
        return;

    TileActor* pTileActor = GetTileActor();
    TileMap* pTileMap = pTileActor->GetComponent<TileMap>();

    Vec2 vColRow = pTileMap->GetColRow(KeyMgr::GetInst()->GetMousePos());

    // 예외처리
    if (vColRow.x < 0.f
        || vColRow.y < 0.f
        || pTileMap->GetCol() <= (int)vColRow.x
        || pTileMap->GetRow() <= (int)vColRow.y)
    {
        return;
    }     
    
    pTileMap->SetSprite(m_CurSprite, (int)vColRow.x, (int)vColRow.y);
}

void EditorLevel::ChangeSprite()
{
    m_CurSprite = AssetMgr::GetInst()->LoadSprite(L"Sprite_01", L"Sprite\\Tile\\TileSprite_0.sprite");    
}
