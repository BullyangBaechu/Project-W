#include "pch.h"
#include "StateMachine.h"

#include "State.h"

StateMachine::StateMachine()
	: Component(COMPONENT_TYPE::STATE_MACHINE)
	, m_CurState(nullptr)
	, m_PrevState(nullptr)
{
}

StateMachine::~StateMachine()
{
	Delete_Map(m_mapState);
}

void StateMachine::FinalTick()
{
	if (nullptr == m_CurState)
		return;

	m_CurState->FinalTick();
}


void StateMachine::AddState(const wstring& _Key, State* _State)
{
	assert(m_mapState.end() == m_mapState.find(_Key));
	m_mapState.insert(make_pair(_Key, _State));
	_State->m_Owner = this;
}

void StateMachine::ChangeState(const wstring& _NextState)
{
	if (nullptr != m_CurState)
		m_CurState->Exit();

	m_PrevState = m_CurState;
	m_CurState = GetState(_NextState);
	// 변경할 스테이트가 존재하지 않는 경우
	assert(m_CurState);

	m_CurState->Enter();
}

void StateMachine::ChangePrevState()
{
	if (nullptr == m_PrevState)
		return;

	m_CurState->Exit();

	State* pTemp = m_CurState;
	m_CurState = m_PrevState;
	m_PrevState = pTemp;	

	m_CurState->Enter();
}
