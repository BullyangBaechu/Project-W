#pragma once


// 사건(이벤트) 를 프레임단위로 동기화 하기 위해서
// 지연처리를 담당하는 매니저
class TaskMgr
{
	SINGLE(TaskMgr);
private:
	list<tTask> m_Task;

public:
	void AddTask(const tTask& _task) { m_Task.push_back(_task); }

public:
	void Tick();
};

