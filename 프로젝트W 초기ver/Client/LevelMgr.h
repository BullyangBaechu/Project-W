#pragma once

class Level;

class LevelMgr
{
	SINGLE(LevelMgr);
private:
	Level*	 m_AllLevel[(UINT)LEVEL_TYPE::END];
	Level*	 m_CurLevel;

public:
	Level* GetCurrentLevel() { return m_CurLevel; }	
	Actor* FindActorByName(const wstring& _Name);
	Actor* FindActorByName(ACTOR_TYPE _Type, const wstring& _Name);
	Actor* GetPlayer();
	

private:
	void ChangeLevel(LEVEL_TYPE _Next);

public:
	void Init();
	void Progress();

	friend class TaskMgr;
};

