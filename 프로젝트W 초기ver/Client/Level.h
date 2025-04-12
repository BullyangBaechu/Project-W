#pragma once


#include "Base.h"

class Actor;

class Level
	: public Base
{
private:
	vector<Actor*>	m_Actor[(UINT)ACTOR_TYPE::END];

	Actor*			m_Player;

public:
	virtual void Begin();
	virtual void Tick();
	virtual void FinalTick();
	virtual void Render(HDC _dc);

	virtual void Enter() = 0;
	virtual void Exit() = 0;

public:
	void AddObject(ACTOR_TYPE _Type, Actor* _Actor);
	const vector<Actor*>& GetObjects(ACTOR_TYPE _Type) { return m_Actor[(UINT)_Type]; }

	void RegisterAsPlayer(Actor* _Actor) { m_Player = _Actor; }
	Actor* GetPlayer() { return m_Player; }

	Actor* FindActorByName(const wstring& _Name);
	Actor* FindActorByName(ACTOR_TYPE _Type, const wstring& _Name);

	void DeleteObject(ACTOR_TYPE _Type);
	void DeleteAllObject();


public:
	Level();
	~Level();

	friend class UIMgr;
};

