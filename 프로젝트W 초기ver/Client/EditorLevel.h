#pragma once
#include "Level.h"

class TileActor;
class Sprite;

class EditorLevel :
    public Level
{
private:
    HMENU       m_hMenu;
    TileActor*  m_TileActor;
    Sprite*     m_CurSprite;

public:
    TileActor* GetTileActor();

private:
    void TileClick();
    void ChangeSprite();
    void CreateEditorUI();

public:
    virtual void Enter() override;
    virtual void Tick() override;
    virtual void Exit() override;

public:
    EditorLevel();
    ~EditorLevel();
};

