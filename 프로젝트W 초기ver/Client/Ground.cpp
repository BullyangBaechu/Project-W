#include "pch.h"
#include "Ground.h"

#include "Actor.h"
#include "Collider.h"

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
}


