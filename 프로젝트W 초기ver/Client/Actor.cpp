#include "pch.h"
#include "Actor.h"

#include "Component.h"

#include "Engine.h"
#include "TaskMgr.h"
#include "Camera.h"

Actor::Actor()
	: m_Pos{}
	, m_Scale{}
	, m_ActorType(ACTOR_TYPE::END)
	, m_Dead(false)
	, m_CamCheck(true)
{
}

Actor::~Actor()
{
	for (size_t i = 0; i < m_vecCom.size(); ++i)
	{
		delete m_vecCom[i];
	}
}

void Actor::FinalTick()
{
	for (size_t i = 0; i < m_vecCom.size(); ++i)
	{
		m_vecCom[i]->FinalTick();
	}
}

void Actor::Render(HDC _dc)
{
	Rectangle(_dc
		, (int)(m_Pos.x - m_Scale.x / 2.f)
		, (int)(m_Pos.y - m_Scale.y / 2.f)
		, (int)(m_Pos.x + m_Scale.x / 2.f)
		, (int)(m_Pos.y + m_Scale.y / 2.f));
}

Vec2 Actor::GetRenderPos()
{
	return Camera::GetInst()->GetRenderPos(m_Pos);	
}

// 자기 휘하로 관리할 액터들 추가
void Actor::AddChild(Actor* child)
{
	m_ChildActor.push_back(child);
}

// 자기 휘하 관리 중인 액터들 삭제
void Actor::RemoveChild(Actor* child)
{
	auto iter = find(m_ChildActor.begin(), m_ChildActor.end(), child);
}

bool Actor::CamCheck()
{
	if (!m_CamCheck)
		return true;

	Vec2 vResol = Engine::GetInst()->GetResolution();
	Vec2 vLookAt = Camera::GetInst()->GetLookAt();
	
	if (   (vLookAt.x - (vResol.x / 2.f) < m_Pos.x + m_Scale.x / 2.f && m_Pos.x - m_Scale.x / 2.f < vLookAt.x + (vResol.x / 2.f)) 
		&& (vLookAt.y - (vResol.y / 2.f) < m_Pos.y + m_Scale.y / 2.f && m_Pos.y - m_Scale.y / 2.f < vLookAt.y + (vResol.y / 2.f)) )
	{
		return true;
	}

	return false;
}

void Actor::Destroy()
{
	if (m_Dead)
		return;

	tTask task = {};

	task.Type = TASK_TYPE::DESTROY_ACTOR;
	task.Param0 = (DWORD_PTR)this;

	TaskMgr::GetInst()->AddTask(task);
}

Component* Actor::GetComponent(COMPONENT_TYPE _Type)
{
	for (size_t i = 0; i < m_vecCom.size(); ++i)
	{
		if (_Type == m_vecCom[i]->GetType())
			return m_vecCom[i];
	}

	return nullptr;
}

Component* Actor::GetComponent(const wstring& _Name)
{
	for (size_t i = 0; i < m_vecCom.size(); ++i)
	{
		if (_Name == m_vecCom[i]->GetName())
			return m_vecCom[i];
	}

	return nullptr;
}
