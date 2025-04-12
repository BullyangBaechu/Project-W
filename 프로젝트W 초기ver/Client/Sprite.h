#pragma once

#include "Asset.h"

class Texture;

// 특정 텍스쳐의 영역을 지정하는 에셋
class Sprite :
    public Asset
{
private:
    Texture*    m_Atlas;
    Vec2        m_LeftTop;
    Vec2        m_Slice;
    Vec2        m_Offset;

public:
    Vec2 GetSlice() { return m_Slice; }
    Vec2 GetLeftTop() { return m_LeftTop; }
    Vec2 GetOffset() { return m_Offset; }
    Texture* GetAltas() { return m_Atlas; }

    void SetOffset(Vec2 _Offset) { m_Offset = _Offset; }

public:
    void Create(Texture* _Atlas, Vec2 _LeftTop, Vec2 _Slice);

    int Save(const wstring& _Path);
    int Load(const wstring& _Path);
   


public:
    Sprite();
    ~Sprite();
};

