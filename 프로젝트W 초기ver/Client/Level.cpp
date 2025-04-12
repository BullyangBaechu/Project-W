#include "pch.h"
#include "Level.h"

#include "Actor.h"

Level::Level()
	: m_Player(nullptr)
{
}

Level::~Level()
{
	for (int i = 0; i < (UINT)ACTOR_TYPE::END; ++i)
	{
		Delete_Vec(m_Actor[i]);
	}
}

void Level::Begin()
{
	for (int i = 0; i < (UINT)ACTOR_TYPE::END; ++i)
	{
		for (int j = 0; j < m_Actor[i].size(); ++j)
		{
			m_Actor[i][j]->Begin();
		}
	}
}

void Level::Tick()
{
	for (int i = 0; i < (UINT)ACTOR_TYPE::END; ++i)
	{
		for (int j = 0; j < m_Actor[i].size(); ++j)
		{
			m_Actor[i][j]->Tick();
		}
	}
}

void Level::FinalTick()
{
	for (int i = 0; i < (UINT)ACTOR_TYPE::END; ++i)
	{
		for (int j = 0; j < m_Actor[i].size(); ++j)
		{
			m_Actor[i][j]->FinalTick();
		}
	}
}

void Level::Render(HDC _dc)
{
	for (int i = 0; i < (UINT)ACTOR_TYPE::END; ++i)
	{
		vector< Actor*>::iterator iter = m_Actor[i].begin();

		for (; iter != m_Actor[i].end();)
		{
			if ((*iter)->IsDead())
			{
				iter = m_Actor[i].erase(iter);
			}
			else
			{
				if ((*iter)->CamCheck())
				{
					(*iter)->Render(_dc);
				}			
				++iter;
			}
		}
	}
}

void Level::AddObject(ACTOR_TYPE _Type, Actor* _Actor)
{
	// Actor 를 지정된 그룹 소속으로 넣어줌
	m_Actor[(UINT)_Type].push_back(_Actor);

	// Actor 가 자신의 소속이 어떤 그룹인지 알게 함
	_Actor->m_ActorType = _Type;
}

Actor* Level::FindActorByName(const wstring& _Name)
{
	for (UINT i = 0; i < (UINT)ACTOR_TYPE::END; ++i)
	{
		Actor* pActor = FindActorByName((ACTOR_TYPE)i, _Name);
		if (nullptr != pActor)
			return pActor;
	}

	return nullptr;
}

Actor* Level::FindActorByName(ACTOR_TYPE _Type, const wstring& _Name)
{
	for (size_t i = 0; i < m_Actor[(UINT)_Type].size(); ++i)
	{
		if (m_Actor[(UINT)_Type][i]->GetName() == _Name)
			return m_Actor[(UINT)_Type][i];
	}

	return nullptr;
}

void Level::DeleteObject(ACTOR_TYPE _Type)
{
	vector<Actor*>& vecActor = m_Actor[(UINT)_Type];

	for (size_t i = 0; i < vecActor.size(); ++i)
	{
		delete vecActor[i];
	}

	vecActor.clear();
}

void Level::DeleteAllObject()
{
	for (UINT i = 0; i < (UINT)ACTOR_TYPE::END; ++i)
	{
		DeleteObject((ACTOR_TYPE)i);
	}
}
