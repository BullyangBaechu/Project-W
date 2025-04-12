#include "pch.h"
#include "Texture.h"

#include "RenderMgr.h"


Texture::Texture()
	: Asset(ASSET_TYPE::TEXTURE)
	, m_Bitmap(nullptr)
	, m_DC(nullptr)
	, m_Info{}
{
}

Texture::~Texture()
{
	DeleteDC(m_DC);
	DeleteObject(m_Bitmap);
}

int Texture::Load(const wstring& _Path)
{
	wchar_t Extention[50] = {};
	_wsplitpath_s(_Path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0 , Extention, 50);



	wstring Ext = Extention;

	if (Ext == L".bmp" || Ext == L".BMP")
	{
		// 파일로부터 비트맵 로딩
		m_Bitmap = (HBITMAP)LoadImage(nullptr, _Path.c_str(), IMAGE_BITMAP
			, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

		if (nullptr == m_Bitmap)
		{
			errno_t ErrNum = GetLastError();

			wchar_t buff[50] = {};
			swprintf_s(buff, 50, L"에러 번호 : %d", ErrNum);
			MessageBox(nullptr, buff, L"텍스쳐 로딩 실패", MB_OK);

			return E_FAIL;
		}
	}

	else if (Ext == L".png" || Ext == L".PNG")
	{
		ULONG_PTR gdiplustoken = 0;
		GdiplusStartupInput gidstartupInput = {};
		GdiplusStartup(&gdiplustoken, &gidstartupInput, nullptr);

		// Image 객체로 Png 포맷 파일을 로딩
		Image* pImg = Image::FromFile(_Path.c_str(), 0);

		// 로딩한 Image 데이터를 Bitmap 형태로 전환
		Bitmap* pBtmap = (Bitmap*)pImg->Clone();
		pBtmap->GetHBITMAP(Color(0.f, 0.f ,0.f, 0.f), &m_Bitmap);

		// 원본 이미지객체 삭제
		delete pImg;
	}

	else
	{
		assert(nullptr);
	}

	// 로딩된 비트맵과 연결할 DC
	m_DC = CreateCompatibleDC(RenderMgr::GetInst()->GetMainDC());

	// 기본 비트맵 삭제 및 DC 와 Bitmap 연결
	DeleteObject(SelectObject(m_DC, m_Bitmap));

	// 생성된 비트맵의 정보를 받아온다.
	GetObject(m_Bitmap, sizeof(BITMAP), &m_Info);

	return S_OK;
}

int Texture::Create(UINT _Width, UINT _Height)
{
	// 실제 화면을 렌더링할 추가 버퍼(비트맵) 와 DC 를 생성
	m_Bitmap = CreateCompatibleBitmap(RenderMgr::GetInst()->GetMainDC(), _Width, _Height);

	// 추가 DC 생성
	m_DC = CreateCompatibleDC(RenderMgr::GetInst()->GetMainDC());

	if (!m_Bitmap || !m_DC)
		return E_FAIL;

	// 새로 생성한 Bitmap 과 DC 를 연결
	// 기존에 들고있던 Bitmap 을 바로 삭제
	DeleteObject(SelectObject(m_DC, m_Bitmap));

	// 생성된 비트맵의 정보를 받아온다.
	GetObject(m_Bitmap, sizeof(BITMAP), &m_Info);

	return S_OK;
}

void Texture::Clear(BRUSH_TYPE _Brush)
{
	SEL_BRUSH(m_DC, _Brush);
	Rectangle(m_DC, -1, -1, m_Info.bmWidth + 1, m_Info.bmHeight + 1);
}