#include "pch.h"
#include "Collider.h"

#include "CollisionMgr.h"
#include "RenderMgr.h"
#include "Actor.h"

#include "Camera.h"

Collider::Collider()
	: Component(COMPONENT_TYPE::COLLIDER)
	, m_Overlap(0)
{
}

Collider::~Collider()
{
}

void Collider::FinalTick()
{
	if (GetOwner() == nullptr)
		return;
	
	// 충돌체의 위치 갱신
	Vec2 vActorPos = GetOwner()->GetPos();
	m_FinalPos = vActorPos + m_OffsetPos;

	// Collider 컴포넌트를 소유한 Actor 의 소속을 확인 후
	// CollisionMgr 에 자신을 소속 그룹에 맞게 등록한다.
	ACTOR_TYPE Type = GetOwner()->GetActorType();
	// 안전하게
	if (Type == ACTOR_TYPE::END)
		return;
	CollisionMgr::GetInst()->RegisterCollider(Type, this);

	// 충돌체의 범위를 화면에 그려준다.

	

	if(m_Overlap)
		DrawDebugRect(Camera::GetInst()->GetRenderPos(m_FinalPos), m_Scale, PEN_TYPE::RED, 0.f);
	else
		DrawDebugRect(Camera::GetInst()->GetRenderPos(m_FinalPos), m_Scale, PEN_TYPE::GREEN, 0.f);
}

void Collider::BeginOverlap(Collider* _Other)
{
	++m_Overlap;
	GetOwner()->BeginOverlap(this, _Other->GetOwner(), _Other);
}

void Collider::Overlap(Collider* _Other)
{	
	GetOwner()->Overlap(this, _Other->GetOwner(), _Other);
}

void Collider::EndOverlap(Collider* _Other)
{
	--m_Overlap;
	GetOwner()->EndOverlap(this, _Other->GetOwner(), _Other);
}
