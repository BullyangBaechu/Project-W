#pragma once
#include "Asset.h"

class Texture :
    public Asset
{
private:
    HBITMAP     m_Bitmap;   // 생성된 비트맵 커널오브젝트 핸들
    HDC         m_DC;       // 생성된 DC 의 핸들(비트맵과 연결 됨)
    BITMAP      m_Info;     // 생성된 비트맵의 정보

   

private:
    int Load(const wstring& _Path);
    int Create(UINT _Width, UINT _Height);


public:
    HBITMAP GetBitmap() { return m_Bitmap; }
    HDC GetDC() { return m_DC; }

    int GetWidth() { return m_Info.bmWidth; }
    int GetHeight() { return m_Info.bmHeight; }

    void Clear(BRUSH_TYPE _Brush);

public:
    Texture();
    ~Texture();

    friend class AssetMgr;
};

