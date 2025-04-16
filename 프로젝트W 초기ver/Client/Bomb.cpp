#include "pch.h"
#include "Bomb.h"
#include "Collider.h"

void Bomb::Init()
{
	SetName(L"Bomb");

	SetScale(Vec2(64.f, 64.f));

	m_Collider = AddComponent(new Collider);
	m_Collider->SetScale(Vec2(100.f, 100.f)); // 테스트 용
	// m_Collider->SetScale(GetScale()); -> 텍스쳐 넣으면 이거 쓰기
}

void Bomb::Tick()
{
}
