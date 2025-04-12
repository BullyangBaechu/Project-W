#include "pch.h"
#include "TileMap.h"

#include "PathMgr.h"

#include "Engine.h"
#include "AssetMgr.h"
#include "KeyMgr.h"

#include "Actor.h"
#include "Sprite.h"
#include "Texture.h"

TileMap::TileMap()
	: Component(COMPONENT_TYPE::TILEMAP)
	, m_Col(0)
	, m_Row(0)
	, m_ShowGrid(true)
{
}

TileMap::~TileMap()
{
}


void TileMap::FinalTick()
{
	// 타일 그리드 선 렌더링
	if(m_ShowGrid)
		DebugRender();
}

void TileMap::Render(HDC _dc)
{
	Vec2 LeftTop = GetColRow(Vec2(0.f, 0.f));
	Vec2 RightBot = GetColRow(Engine::GetInst()->GetResolution());

	if (LeftTop.x < 0.f)	
		LeftTop.x = 0.f;
	if (m_Col <= (int)RightBot.x)
		RightBot.x = (float)m_Col - 1.f;

	if (LeftTop.y < 0.f)
		LeftTop.y = 0.f;
	if (m_Col <= (int)RightBot.y)
		RightBot.y = (float)m_Row - 1.f;

	Vec2 vOwnerPos = GetOwner()->GetRenderPos();

	for (int Row = (int)LeftTop.y; Row <= (int)RightBot.y; ++Row)
	{
		for (int Col = (int)LeftTop.x; Col <= (int)RightBot.x; ++Col)
		{
			int Idx = Row * m_Col + Col;

			if (nullptr == m_vecTileInfo[Idx].Sprite)
				continue;
			
			Vec2 TileLeftTop = vOwnerPos + Vec2(Col * m_TileSize.x, Row * m_TileSize.y);

			BitBlt(_dc
				, (int)TileLeftTop.x
				, (int)TileLeftTop.y
				, (int)m_TileSize.x
				, (int)m_TileSize.y
				, m_vecTileInfo[Idx].Sprite->GetAltas()->GetDC()
				, (int)m_vecTileInfo[Idx].Sprite->GetLeftTop().x
				, (int)m_vecTileInfo[Idx].Sprite->GetLeftTop().y
				, SRCCOPY);
		}
	}
}

void TileMap::SetColRow(int _Col, int _Row)
{
	m_Col = _Col;
	m_Row = _Row;		
	m_vecTileInfo.resize(m_Col * m_Row);

	// 공간이 늘어났다가 줄어든 경우를 대비해서 필요한만큼만 메모리를 사용하기
	if (m_vecTileInfo.size() < m_vecTileInfo.capacity())
	{
		vector<tTileInfo> vecTemp;
		vecTemp.resize(m_vecTileInfo.size());
		vecTemp.swap(m_vecTileInfo);
	}
	
	// 타일 행렬 개수가 변형되기 이전의 상황이, 바뀐 이후에도 유지되려면
	// 각 타일정보 인덱스가 알맞게 변경되어야 한다.
}

void TileMap::SetSprite(Sprite* _Sprite, int _Col, int _Row)
{
	int Idx = (m_Col * _Row) + _Col;
	m_vecTileInfo[Idx].Sprite = _Sprite;
}


void TileMap::DebugRender()
{
	Vec2 vActorPos = GetOwner()->GetRenderPos();

	Vec2 LeftTop = GetColRow(Vec2(0.f, 0.f));
	Vec2 RightBot = GetColRow(Engine::GetInst()->GetResolution());

	if (LeftTop.x < 0.f)
		LeftTop.x = 0.f;
	if (m_Col <= (int)RightBot.x)
		RightBot.x = (float)m_Col - 1.f;

	if (LeftTop.y < 0.f)
		LeftTop.y = 0.f;
	if (m_Col <= (int)RightBot.y)
		RightBot.y = (float)m_Row - 1.f;

	for (int Row = (int)LeftTop.y; Row < (int)RightBot.y + 2; ++Row)
	{
		Vec2 vStart = vActorPos + Vec2(0.f, Row * m_TileSize.y);
		Vec2 vEnd = vStart + Vec2(m_Col * m_TileSize.x, 0.f);
		DrawDebugLine(vStart, vEnd, PEN_TYPE::GREEN, 0.f);
	}

	for (int Col = (int)LeftTop.x; Col < (int)RightBot.x + 2; ++Col)
	{
		Vec2 vStart = vActorPos + Vec2(Col * m_TileSize.x, 0.f);
		Vec2 vEnd = vStart + Vec2(0.f, m_Row * m_TileSize.y);
		DrawDebugLine(vStart, vEnd, PEN_TYPE::GREEN, 0.f);
	}
}


Vec2 TileMap::GetColRow(Vec2 _ScreenPos)
{
	Vec2 vOwnerPos = GetOwner()->GetRenderPos();

	Vec2 vDiff = _ScreenPos - vOwnerPos;

	int Col = (int)(vDiff.x / m_TileSize.x);
	int Row = (int)(vDiff.y / m_TileSize.y);

	// 0 ~ -TileSize 까지 범위도 0 열 or 0 행으로 몫이 나오기 때문에 이부분을 보정해줌
	if (Col == 0 && vDiff.x < 0.f)
		Col = -1;
	if (Row == 0 && vDiff.y < 0.f)
		Row = -1;

	return Vec2(Col, Row);
}

void TileMap::Save(const wstring& _RelativePath)
{
	wstring FilePath = PathMgr::GetInst()->GetContentPath();
	FilePath += _RelativePath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, FilePath.c_str(), L"wb");

	fwrite(&m_Col, sizeof(int), 1, pFile);
	fwrite(&m_Row, sizeof(int), 1, pFile);
	fwrite(&m_TileSize, sizeof(Vec2), 1, pFile);

	int TileCount = m_Col * m_Row;
	for (int i = 0; i < TileCount; ++i)
	{
		fwrite(&m_vecTileInfo[i].bBlock, sizeof(bool), 1, pFile);		
		SaveAsset(m_vecTileInfo[i].Sprite, pFile);
	}

	fclose(pFile);
}

void TileMap::Load(const wstring& _RelativePath)
{
	wstring FilePath = PathMgr::GetInst()->GetContentPath();
	FilePath += _RelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, FilePath.c_str(), L"rb");

	fread(&m_Col, sizeof(int), 1, pFile);
	fread(&m_Row, sizeof(int), 1, pFile);
	fread(&m_TileSize, sizeof(Vec2), 1, pFile);

	SetColRow(m_Col, m_Row);

	int TileCount = m_Col * m_Row;
	for (int i = 0; i < TileCount; ++i)
	{
		fread(&m_vecTileInfo[i].bBlock, sizeof(bool), 1, pFile);
		m_vecTileInfo[i].Sprite = (Sprite*)LoadAsset(ASSET_TYPE::SPRITE, pFile);
	}

	fclose(pFile);
}