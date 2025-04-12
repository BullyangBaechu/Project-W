#include "pch.h"
#include "TaskMgr.h"

#include "LevelMgr.h"
#include "Level.h"
#include "Actor.h"

TaskMgr::TaskMgr()
{

}

TaskMgr::~TaskMgr()
{

}

void TaskMgr::Tick()
{
	list<tTask>::iterator iter = m_Task.begin();

	size_t MaxSize = m_Task.size();
	for (size_t size = 0 ; size < MaxSize; ++size)
	{
		switch (iter->Type)
		{
		case TASK_TYPE::CREATE_ACTOR:
		{
			Level* pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject( (ACTOR_TYPE)iter->Param1
								, (Actor*)iter->Param0);

			((Actor*)iter->Param0)->Begin();
		}
			break;
		case TASK_TYPE::DESTROY_ACTOR:
		{
			Actor* pActor = (Actor*)iter->Param0;

			if (!pActor->m_Dead)
			{
				pActor->m_Dead = true;
				tTask task = {};
				task.Type = TASK_TYPE::DELETE_ACTOR;
				task.Param0 = iter->Param0;
				m_Task.push_back(task);
			}
		}
			break;
		case TASK_TYPE::DELETE_ACTOR:
		{
			Actor* pActor = (Actor*)iter->Param0;
			delete pActor;
		}
			break;
		case TASK_TYPE::CHANGE_LEVEL:
		{
			LEVEL_TYPE Next = (LEVEL_TYPE)iter->Param0;
			LevelMgr::GetInst()->ChangeLevel(Next);
		}
			break;
		}

		iter = m_Task.erase(iter);
	}	
}

