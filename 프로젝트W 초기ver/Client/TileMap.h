#pragma once
#include "Component.h"

class Sprite;

struct tTileInfo
{
    Sprite*     Sprite;
    bool        bBlock;
};

class TileMap :
    public Component
{
private:
    int                 m_Col;          // 열
    int                 m_Row;          // 행
    Vec2                m_TileSize;     // 타일 크기이자 사용할 스프라이트의 크기
    vector<tTileInfo>   m_vecTileInfo;  // 모든 타일 정보

    bool                m_ShowGrid;

public:
    void SetColRow(int _Col, int _Row);
    void SetTileSize(Vec2 _Size) { m_TileSize = _Size; }
    void SetSprite(Sprite* _Sprite, int _Col, int _Row);

    Vec2 GetTileSize() { return m_TileSize; }


    // 픽셀위치에 해당하는 타일의 (열, 행) 정보
    Vec2 GetColRow(Vec2 _ScreenPos);

    UINT GetCol() { return m_Col; }
    UINT GetRow() { return m_Row; }

    void ShowGrid(bool _Show) { m_ShowGrid = _Show; }

private:
    void DebugRender();

public:
    virtual void FinalTick() override;
    void Render(HDC _dc);

    void Save(const wstring& _RelativePath);
    void Load(const wstring& _RelativePath);

public:
    TileMap();
    ~TileMap();
};

