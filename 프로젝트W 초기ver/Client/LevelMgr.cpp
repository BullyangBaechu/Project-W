#include "pch.h"
#include "LevelMgr.h"


#include "StartLevel.h"
#include "RunningLevel.h"
#include "EditorLevel.h"
#include "TestLevel.h"



LevelMgr::LevelMgr()
	: m_CurLevel(nullptr)
	, m_AllLevel{}
{

}

LevelMgr::~LevelMgr()
{
	Delete_Arr(m_AllLevel);
}

void LevelMgr::Init()
{
	m_AllLevel[(UINT)LEVEL_TYPE::START] = new StartLevel;
	m_AllLevel[(UINT)LEVEL_TYPE::RUNNING] = new RunningLevel;
	m_AllLevel[(UINT)LEVEL_TYPE::EDITOR] = new EditorLevel;
	m_AllLevel[(UINT)LEVEL_TYPE::TEST] = new TestLevel;

	::ChangeLevel(LEVEL_TYPE::START);
}

void LevelMgr::Progress()
{
	if (nullptr == m_CurLevel)
		return;

	// 1 프레임동안 Level 안에 있는 Actor 이 할 일을 수행한다.
	m_CurLevel->Tick();

	// Actor 들이 할 일을 마무리 한 뒤, 후속조치
	m_CurLevel->FinalTick();
}

Actor* LevelMgr::FindActorByName(const wstring& _Name)
{
	return m_CurLevel->FindActorByName(_Name);
}

Actor* LevelMgr::FindActorByName(ACTOR_TYPE _Type, const wstring& _Name)
{
	return  m_CurLevel->FindActorByName(_Type, _Name);
}

Actor* LevelMgr::GetPlayer()
{
	return m_CurLevel->GetPlayer();
}

void LevelMgr::ChangeLevel(LEVEL_TYPE _Next)
{
	assert(m_AllLevel[(UINT)_Next] != m_CurLevel);

	if (m_CurLevel)
		m_CurLevel->Exit();

	// 다음 레벨로 교체
	m_CurLevel = m_AllLevel[(UINT)_Next];	

	// 교체후 초기화 작업
	m_CurLevel->Enter();

	// 레벨안에 있는 Actor 들 Begin 호출
	m_CurLevel->Begin();
}
