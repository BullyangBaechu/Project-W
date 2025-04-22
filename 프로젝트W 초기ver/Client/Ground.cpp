#include "pch.h"
#include "Ground.h"

#include "Actor.h"
#include "Collider.h"
#include "Camera.h"
#include "Engine.h"

Ground::Ground()
	: m_Collider(nullptr)
{
	SetName(L"Ground");

	m_Collider = AddComponent(new Collider);
	m_Collider->SetScale(Vec2(80000.f, 100.f));
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

}


