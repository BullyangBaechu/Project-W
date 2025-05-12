#include "pch.h"
#include "Ground.h"

#include "Actor.h"
#include "Collider.h"
#include "Camera.h"
#include "Engine.h"
#include "AssetMgr.h"
#include "Texture.h"

Ground::Ground()
	: m_Collider(nullptr)
	, m_Tex(nullptr)
	, BGidx(2)
{
	SetName(L"Ground");

	m_Tex = AssetMgr::GetInst()->LoadTexture(L"Ground", L"Texture\\IndustrialGround_1_1440x96.bmp");
	m_Collider = AddComponent(new Collider);
	m_Collider->SetScale(Vec2(1440.f, 96.f));
	//m_Collider->SetScale(GetScale());
	
}

Ground::~Ground()
{
}

void Ground::Tick()
{
	
	Vec2 GroundPos = GetPos();
	Vec2 CamPos = Camera::GetInst()->GetLookAt();
	Vec2 Res = Engine::GetInst()->GetResolution();

	// 카메라 기준 X축 경계
	float CamLeft_X = CamPos.x - (Res.x / 2);

	
	// Ground의 오른쪽 끝이 카메라 기준 왼쪽으로 벗어나면
	if (GroundPos.x + GetScale().x / 2< CamLeft_X)
	{
		// BGidx만큼 뒤에 새로 반복해서 나타나게 하기
		GroundPos.x += GetScale().x * BGidx;
		SetPos(GroundPos);
	}
	// + GetScale().x / 2 
	
	
}

void Ground::Render(HDC _dc)
{

	if (m_Tex)
	{
		Vec2 renderPos = GetRenderPos();
		float w = m_Tex->GetWidth();
		float h = m_Tex->GetHeight();

		
		TransparentBlt(_dc,
			(int)(renderPos.x - w / 2.f),
			(int)(renderPos.y - h / 2.f),
			(int)w,
			(int)h,
			m_Tex->GetDC(),
			0, 0,
			(int)w,
			(int)h,
			RGB(255, 0, 255)); // 마젠타 투명

		
	}

	//Actor::Render(_dc); // 부모의 디버그 렌더링 호출
}


