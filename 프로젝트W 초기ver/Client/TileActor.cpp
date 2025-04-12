#include "pch.h"
#include "TileActor.h"

#include "TileMap.h"

TileActor::TileActor()
	: m_TileMap(nullptr)
{
	m_TileMap = AddComponent(new TileMap);
	m_TileMap->SetColRow(0, 0);
	m_TileMap->SetTileSize(Vec2(1.f, 1.f));
}

TileActor::~TileActor()
{

}

void TileActor::ShowGrid(bool _Show)
{
	m_TileMap->ShowGrid(_Show);
}

void TileActor::Tick()
{

}

void TileActor::Render(HDC _dc)
{
	m_TileMap->Render(_dc);
}