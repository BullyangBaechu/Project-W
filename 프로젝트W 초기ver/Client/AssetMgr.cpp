#include "pch.h"
#include "AssetMgr.h"

#include "PathMgr.h"
#include "Texture.h"
#include "Sprite.h"
#include "Flipbook.h"
#include "Sound.h"

#include "Engine.h"

AssetMgr::AssetMgr()
{
}

AssetMgr::~AssetMgr()
{
    Delete_Map(m_mapTex);
    Delete_Map(m_mapSprite);
    Delete_Map(m_mapFlipbook);
    Delete_Map(m_mapSound);
}

void AssetMgr::Init()
{
    InitSound();

    // 타일용 Atlas 텍스쳐 로딩
    Texture* pTileAtlas = LoadTexture(L"TileAtlas", L"Texture\\TILE.bmp");

    int Idx = 0;
    for (int Row = 0; Row < 6; ++Row)
    {
        for (int Col = 0; Col < 8; ++Col, ++Idx)
        {
			Sprite* pSprite = new Sprite;
			pSprite->Create(pTileAtlas, Vec2(Col * 64.f, Row * 64.f), Vec2(64.f, 64.f));

			wchar_t szNum[10] = {};
			_itow_s(Idx, szNum, 10);
            wstring Key = L"TileSprite";
            Key = Key + L"_" + szNum;

			pSprite->SetKey(Key);			
			m_mapSprite.insert(make_pair(Key, pSprite));

			// Sprite 를 파일로 저장
            wstring RelativePath = L"Sprite\\";
            RelativePath = RelativePath + L"Tile\\" + Key + L".sprite";
			pSprite->SetPath(RelativePath);

            wstring ContentPath = PathMgr::GetInst()->GetContentPath();
			pSprite->Save(ContentPath + pSprite->GetPath());
        }
    }


    // Atlas 로 사용할 텍스쳐 로딩
    //Texture* pAtlas = LoadTexture(L"LinkAtlas", L"Texture\\link_32.bmp");

    //CreateFlipbook(pAtlas, L"PLAYER_IDLE_DOWN" , 3, Vec2(0.f, 0.f),   Vec2(120.f, 130.f));
    //CreateFlipbook(pAtlas, L"PLAYER_IDLE_LEFT" , 3, Vec2(0.f, 130.f), Vec2(120.f, 130.f));
    //CreateFlipbook(pAtlas, L"PLAYER_IDLE_UP"   , 1, Vec2(0.f, 260.f), Vec2(120.f, 130.f));
    //CreateFlipbook(pAtlas, L"PLAYER_IDLE_RIGHT", 3, Vec2(0.f, 390.f), Vec2(120.f, 130.f));
    //
    //CreateFlipbook(pAtlas, L"PLAYER_MOVE_DOWN" , 10, Vec2(0.f, 520.f), Vec2(120.f, 130.f));
    //CreateFlipbook(pAtlas, L"PLAYER_MOVE_LEFT" , 10, Vec2(0.f, 650.f), Vec2(120.f, 130.f));
    //CreateFlipbook(pAtlas, L"PLAYER_MOVE_UP"   , 10, Vec2(0.f, 780.f), Vec2(120.f, 130.f));
    //CreateFlipbook(pAtlas, L"PLAYER_MOVE_RIGHT", 10, Vec2(0.f, 910.f), Vec2(120.f, 130.f));
}

void AssetMgr::RegisterToBGM(Sound* _BGM)
{
    if (nullptr != m_BGM)
        m_BGM->Stop();

    m_BGM = _BGM;
}

void AssetMgr::InitSound()
{
    if (FAILED(DirectSoundCreate8(NULL, &m_SoundDevice, NULL)))
    {
        MessageBox(NULL, L"사운드 디바이스 생성 실패", L"SYSTEM ERROR", MB_OK);
        return;
    }

    // 사운드 협조레벨 설정
    HWND hWnd = Engine::GetInst()->GetMainWnd();
    if (FAILED(m_SoundDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE)))
    {
        MessageBox(NULL, L"사운드 매니저 초기화 실패", L"SYSTEM ERROR", MB_OK);
    }
}

void AssetMgr::CreateFlipbook(Texture* _pAtlas, const wstring& _Name, int _SpriteCount, Vec2 _LeftTop, Vec2 _Slice)
{
    wstring ContentPath = PathMgr::GetInst()->GetContentPath();

    // Sprite 제작
    for (int i = 0; i < _SpriteCount; ++i)
    {
        //Sprite* pSprite = new Sprite;
        //pSprite->Create(_pAtlas, _LeftTop + Vec2(i * 120.f, 0.f), _Slice);

        //wchar_t szNum[10] = {};
        //_itow_s(i, szNum, 10);

        //wstring Key = _Name + L"_" + szNum;       

        //pSprite->SetKey(Key);
        ////pSprite->SetPath();
        //m_mapSprite.insert(make_pair(Key, pSprite));

        //// Sprite 를 파일로 저장
        //pSprite->SetPath(L"Sprite\\" + Key + L".sprite");
        //pSprite->Save(ContentPath + pSprite->GetPath());
                
        // 불러오기
        wchar_t szNum[10] = {};
        _itow_s(i, szNum, 10);
        wstring Key = _Name + L"_" + szNum;    

        wstring Path = L"Sprite\\" + Key + L".sprite";
        LoadSprite(Key, Path);
    }


    // Flipbook 제작
    Flipbook* pFlipbook = new Flipbook;

    for (int i = 0; i < _SpriteCount; ++i)
    {
        wchar_t szNum[10] = {};
        _itow_s(i, szNum, 10);
        wstring Key = _Name + L"_" + szNum;

        Sprite* pSprite = FindSprite(Key);

        pFlipbook->AddSprite(pSprite);
    }

    pFlipbook->SetKey(_Name);
    //pSprite->SetPath();
    m_mapFlipbook.insert(make_pair(_Name, pFlipbook));

    // Flipbook 저장
    pFlipbook->SetPath(L"Flipbook\\" + pFlipbook->GetKey() + L".flip");
    pFlipbook->Save(ContentPath + pFlipbook->GetPath());
}

Texture* AssetMgr::FindTexture(const wstring& _Key)
{
    map<wstring, Texture*>::iterator iter = m_mapTex.find(_Key);

    if (iter == m_mapTex.end())
    {
        return nullptr;
    }

    return iter->second;
}


Texture* AssetMgr::LoadTexture(const wstring& _Key, const wstring& _RelativePath)
{
    // 해당 에셋이 있는지 찾아본다.
    Texture* pTex = FindTexture(_Key);

    // 이미 동일한 키값으로 등록된 텍스쳐가 있으면 해당 텍스쳐를 반환한다.
    if (nullptr != pTex)
        return pTex;
    
    // 없으면 로딩해서 에셋매니저에 등록 후 반환한다.
    wstring FilePath = PathMgr::GetInst()->GetContentPath();
    FilePath += _RelativePath;

    pTex = new Texture;
    if (FAILED(pTex->Load(FilePath)))    
    {
        delete pTex;
        return nullptr;
    }


    pTex->SetKey(_Key);
    pTex->SetPath(_RelativePath);
    m_mapTex.insert(make_pair(_Key, pTex));

    return pTex;
}

Texture* AssetMgr::CreateTexture(const wstring& _Key, UINT _Width, UINT _Height)
{
    // 해당 에셋이 있는지 찾아본다.
    Texture* pTex = FindTexture(_Key);

    // 이미 동일한 키값으로 등록된 텍스쳐가 있으면 해당 텍스쳐를 반환한다.
    if (nullptr != pTex)
        return pTex;

    // 없으면 직접 생성해서 에셋매니저에 등록 후 반환한다.  
    pTex = new Texture;
    if (FAILED(pTex->Create(_Width, _Height)))
    {
        delete pTex;
        return nullptr;
    }

    pTex->SetKey(_Key);
    m_mapTex.insert(make_pair(_Key, pTex));

    return pTex;
}

Sprite* AssetMgr::FindSprite(const wstring& _Key)
{
    map<wstring, Sprite*>::iterator iter = m_mapSprite.find(_Key);

    if (iter == m_mapSprite.end())
    {
        return nullptr;
    }

    return iter->second;
}

Sprite* AssetMgr::LoadSprite(const wstring& _Key, const wstring& _RelativePath)
{
    // 해당 에셋이 있는지 찾아본다.
    Sprite* pSprite = FindSprite(_Key);

    // 이미 동일한 키값으로 등록된 텍스쳐가 있으면 해당 텍스쳐를 반환한다.
    if (nullptr != pSprite)
        return pSprite;

    // 없으면 로딩해서 에셋매니저에 등록 후 반환한다.
    wstring FilePath = PathMgr::GetInst()->GetContentPath();
    FilePath += _RelativePath;

    pSprite = new Sprite;
    if (FAILED(pSprite->Load(FilePath)))
    {
        delete pSprite;
        return nullptr;
    }


    pSprite->SetKey(_Key);
    pSprite->SetPath(_RelativePath);
    m_mapSprite.insert(make_pair(_Key, pSprite));

    return pSprite;
}

Flipbook* AssetMgr::FindFlipbook(const wstring& _Key)
{
    map<wstring, Flipbook*>::iterator iter = m_mapFlipbook.find(_Key);

    if (iter == m_mapFlipbook.end())
    {
        return nullptr;
    }

    return iter->second;
}

Flipbook* AssetMgr::LoadFlipbook(const wstring& _Key, const wstring& _RelativePath)
{
    // 해당 에셋이 있는지 찾아본다.
    Flipbook* pFlipbook = FindFlipbook(_Key);

    // 이미 동일한 키값으로 등록된 텍스쳐가 있으면 해당 텍스쳐를 반환한다.
    if (nullptr != pFlipbook)
        return pFlipbook;

    // 없으면 로딩해서 에셋매니저에 등록 후 반환한다.
    wstring FilePath = PathMgr::GetInst()->GetContentPath();
    FilePath += _RelativePath;

    pFlipbook = new Flipbook;
    if (FAILED(pFlipbook->Load(FilePath)))
    {
        delete pFlipbook;
        return nullptr;
    }


    pFlipbook->SetKey(_Key);
    pFlipbook->SetPath(_RelativePath);
    m_mapFlipbook.insert(make_pair(_Key, pFlipbook));

    return pFlipbook;
}

Sound* AssetMgr::FindSound(const wstring& _Key)
{
    map<wstring, Sound*>::iterator iter = m_mapSound.find(_Key);

    if (iter == m_mapSound.end())
    {
        return nullptr;
    }

    return iter->second;
}

Sound* AssetMgr::LoadSound(const wstring& _Key, const wstring& _RelativePath)
{
    // 해당 에셋이 있는지 찾아본다.
    Sound* pSound = FindSound(_Key);

    // 이미 동일한 키값으로 등록된 텍스쳐가 있으면 해당 텍스쳐를 반환한다.
    if (nullptr != pSound)
        return pSound;

    // 없으면 로딩해서 에셋매니저에 등록 후 반환한다.
    wstring FilePath = PathMgr::GetInst()->GetContentPath();
    FilePath += _RelativePath;

    pSound = new Sound;
    if (FAILED(pSound->Load(FilePath)))
    {
        delete pSound;
        return nullptr;
    }

    pSound->SetKey(_Key);
    pSound->SetPath(_RelativePath);
    m_mapSound.insert(make_pair(_Key, pSound));

    return pSound;
}
