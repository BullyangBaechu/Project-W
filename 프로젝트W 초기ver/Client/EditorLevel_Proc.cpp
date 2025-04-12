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

// ====
// Proc
// ====
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

bool CreateTileCountDlg(UINT message, WPARAM _wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(_wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_TILE_COUNT:
            KeyMgr::GetInst()->SetLock(1.f);

            DialogBox(Engine::GetInst()->GetMainInst(), MAKEINTRESOURCE(DLG_TILE_COUNT)
                , Engine::GetInst()->GetMainWnd(), TileCountProc);

            return true;
        case ID_TILE_SAVE:
        {
            // 현재 레벨이 EditorLevel 이 아니라면
            EditorLevel* pLevel = dynamic_cast<EditorLevel*>(LevelMgr::GetInst()->GetCurrentLevel());
            assert(pLevel);

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
                    break;

                // 앞부분 ContentPath 버리고, 뒷부분 상대경로만 잘라냄
                int ContentPathLen = wcslen(PathMgr::GetInst()->GetContentPath());
                wstring RelativePath = strFilePath.substr(ContentPathLen, strFilePath.length());

                // 저장
                pTileMap->Save(RelativePath);
            }
        }
        break;
        case ID_TILE_LOAD:
        {
            // 현재 레벨이 EditorLevel 이 아니라면
            EditorLevel* pLevel = dynamic_cast<EditorLevel*>(LevelMgr::GetInst()->GetCurrentLevel());
            assert(pLevel);

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
                    break;

                // 앞부분 ContentPath 버리고, 뒷부분 상대경로만 잘라냄
                int ContentPathLen = wcslen(PathMgr::GetInst()->GetContentPath());
                wstring RelativePath = strFilePath.substr(ContentPathLen, strFilePath.length());

                pTileMap->Load(RelativePath);
            }
        }
        break;
        }
    }
    }

    return false;
}


// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        // 현재 레벨이 EditorLevel 이 아니라면
        EditorLevel* pLevel = dynamic_cast<EditorLevel*>(LevelMgr::GetInst()->GetCurrentLevel());
        assert(pLevel);

        TileActor* pActor = pLevel->GetTileActor();
        TileMap* pTileMap = pActor->GetComponent<TileMap>();

        int MaxCol = pTileMap->GetCol();
        int MaxRow = pTileMap->GetRow();

        SetDlgItemInt(hDlg, IDC_EDIT1, MaxCol, true);
        SetDlgItemInt(hDlg, IDC_EDIT2, MaxRow, true);

        Vec2 vSize = pTileMap->GetTileSize();

        SetDlgItemInt(hDlg, IDC_EDIT3, vSize.x, true);
        SetDlgItemInt(hDlg, IDC_EDIT4, vSize.y, true);
    }
    return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            int Col = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, true);
            int Row = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, true);

            int SizeX = GetDlgItemInt(hDlg, IDC_EDIT3, nullptr, true);
            int SizeY = GetDlgItemInt(hDlg, IDC_EDIT4, nullptr, true);

            // 현재 레벨이 EditorLevel 이 아니라면
            EditorLevel* pLevel = dynamic_cast<EditorLevel*>(LevelMgr::GetInst()->GetCurrentLevel());
            assert(pLevel);

            TileActor* pActor = pLevel->GetTileActor();
            TileMap* pTileMap = pActor->GetComponent<TileMap>();
            pTileMap->SetColRow(Col, Row);
            pTileMap->SetTileSize(Vec2(SizeX, SizeY));

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}