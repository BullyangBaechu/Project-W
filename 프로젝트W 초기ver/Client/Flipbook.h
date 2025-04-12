#pragma once
#include "Asset.h"

class Sprite;

class Flipbook :
    public Asset
{
private:
    vector<Sprite*>     m_Sprites;


public:
    void AddSprite(Sprite* _Sprite, int _Idx = -1);
    Sprite* GetSprite(int _Idx) { return m_Sprites[_Idx]; }
    int GetMaxSpriteCount() { return (int)m_Sprites.size(); }

    int Save(const wstring& _Path);
    int Load(const wstring& _Path);

public:
    Flipbook();
    ~Flipbook();
};

