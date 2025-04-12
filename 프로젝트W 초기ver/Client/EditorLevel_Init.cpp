#include "pch.h"
#include "EditorLevel.h"

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

void EditorLevel::CreateEditorUI()
{
	{
		// PanelUI 생성
		PanelUI* pPanel = new PanelUI;
		pPanel->SetPos(Vec2(780.f, 50.f));
		pPanel->SetScale(Vec2(400.f, 500.f));
		AddObject(ACTOR_TYPE::UI, pPanel);

		// SaveButton 을 PanelUI 자식으로 추가
		BtnUI* pBtnUI = new BtnUI;
		pBtnUI->SetName(L"Save Button");
		pBtnUI->SetPos(Vec2(10.f, 10.f));

		pBtnUI->SetBaseImage(AssetMgr::GetInst()->LoadTexture(L"BtnImage", L"Texture\\Start.png"));
		pBtnUI->SetHoverImage(nullptr);
		pBtnUI->SetPressedImage(nullptr);
		void PressSaveBtn();
		pBtnUI->SetCallBack(&PressSaveBtn);

		pPanel->AddChildUI(pBtnUI);

		pBtnUI = new BtnUI;
		pBtnUI->SetName(L"Load Button");
		pBtnUI->SetBaseImage(AssetMgr::GetInst()->LoadTexture(L"BtnImage", L"Texture\\Start.png"));
		pBtnUI->SetHoverImage(nullptr);
		pBtnUI->SetPressedImage(nullptr);
		pBtnUI->SetPos(Vec2(400.f - pBtnUI->GetScale().x - 10, 10.f));
		void PressLoadBtn();
		pBtnUI->SetCallBack(&PressLoadBtn);
		pPanel->AddChildUI(pBtnUI);
	}

	{
		// PanelUI 생성
		PanelUI* pPanel = new PanelUI;
		pPanel->SetPos(Vec2(280.f, 50.f));
		pPanel->SetScale(Vec2(400.f, 500.f));
		AddObject(ACTOR_TYPE::UI, pPanel);

		// SaveButton 을 PanelUI 자식으로 추가
		BtnUI* pBtnUI = new BtnUI;
		pBtnUI->SetName(L"Save Button");
		pBtnUI->SetPos(Vec2(10.f, 10.f));

		pBtnUI->SetBaseImage(AssetMgr::GetInst()->LoadTexture(L"BtnImage", L"Texture\\Start.png"));
		pBtnUI->SetHoverImage(nullptr);
		pBtnUI->SetPressedImage(nullptr);
		void PressSaveBtn();
		pBtnUI->SetCallBack(&PressSaveBtn);

		pPanel->AddChildUI(pBtnUI);

		pBtnUI = new BtnUI;
		pBtnUI->SetName(L"Load Button");
		pBtnUI->SetBaseImage(AssetMgr::GetInst()->LoadTexture(L"BtnImage", L"Texture\\Start.png"));
		pBtnUI->SetHoverImage(nullptr);
		pBtnUI->SetPressedImage(nullptr);
		pBtnUI->SetPos(Vec2(400.f - pBtnUI->GetScale().x - 10, 10.f));
		void PressLoadBtn();
		pBtnUI->SetCallBack(&PressLoadBtn);
		pPanel->AddChildUI(pBtnUI);
	}
}


// ========
// CallBack
// ========
void PressSaveBtn()
{
	// 현재 레벨이 EditorLevel 이 아니라면
	EditorLevel* pLevel = dynamic_cast<EditorLevel*>(LevelMgr::GetInst()->GetCurrentLevel());
	if (nullptr == pLevel)
		return;

	TileActor* pActor = pLevel->GetTileActor();
	TileMap* pTileMap = pActor->GetComponent<TileMap>();

	// 탐색기 다이얼로그 실행
	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};
	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;	// 최종적으로 고른 경로를 받아낼 목적지
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"TileMap\0*.tilemap\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = PathMgr::GetInst()->GetContentPath();

	if (GetSaveFileName(&Desc))
	{
		// Content 폴더 하위경로가 지정되었는지 확인
		wstring strFilePath = szFilePath;
		if (wstring::npos == strFilePath.find(PathMgr::GetInst()->GetContentPath()))
			return;

		// 앞부분 ContentPath 버리고, 뒷부분 상대경로만 잘라냄
		int ContentPathLen = wcslen(PathMgr::GetInst()->GetContentPath());
		wstring RelativePath = strFilePath.substr(ContentPathLen, strFilePath.length());

		// 저장
		pTileMap->Save(RelativePath);
	}
}

void PressLoadBtn()
{
	// 현재 레벨이 EditorLevel 이 아니라면
	EditorLevel* pLevel = dynamic_cast<EditorLevel*>(LevelMgr::GetInst()->GetCurrentLevel());
	if (nullptr == pLevel)
		return;

	TileActor* pActor = pLevel->GetTileActor();
	TileMap* pTileMap = pActor->GetComponent<TileMap>();

	// 탐색기 다이얼로그
	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"TileMap\0*.tilemap\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = PathMgr::GetInst()->GetContentPath();

	if (GetOpenFileName(&Desc))
	{
		// Content 폴더 하위경로가 지정되었는지 확인
		wstring strFilePath = szFilePath;
		if (wstring::npos == strFilePath.find(PathMgr::GetInst()->GetContentPath()))
			return;

		// 앞부분 ContentPath 버리고, 뒷부분 상대경로만 잘라냄
		int ContentPathLen = wcslen(PathMgr::GetInst()->GetContentPath());
		wstring RelativePath = strFilePath.substr(ContentPathLen, strFilePath.length());

		pTileMap->Load(RelativePath);
	}
}