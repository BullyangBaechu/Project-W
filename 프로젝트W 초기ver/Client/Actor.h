#pragma once

#include "Base.h"

class Component;
class Collider;

class Actor
	: public Base
{
private:
	Vec2				m_Pos;			// 위치
	Vec2				m_Scale;		// 크기
	vector<Component*>	m_vecCom;		// 컴포넌트
	ACTOR_TYPE			m_ActorType;	// 소속 그룹

	vector<Actor*>		m_ChildActor;	// 자기 휘하 액터 (Spawner 같은 자식 액터가 있는 경우에 사용)


	bool				m_CamCheck;
	bool				m_Dead;			// Actor 의 Life 상태
	bool				m_DontDestroy;

public:
	void SetPos(Vec2 _Pos) { m_Pos = _Pos; }
	void SetScale(Vec2 _Scale) { m_Scale = _Scale; }
	void SetCamCheck(bool _Check) { m_CamCheck = _Check; }

	Vec2 GetPos() const { return m_Pos; }
	Vec2 GetScale() const { return m_Scale; }
	Vec2 GetRenderPos();

	ACTOR_TYPE GetActorType() const { return m_ActorType; }

	// 액터 본인의 휘하 자식 액터 관리용 함수
	void AddChild(Actor* child);
	void RemoveChild(Actor* child);

	bool IsDead() const { return m_Dead ; }
	virtual bool CamCheck();

	void Destroy();

	template<typename T>
	T* AddComponent(T* _Component);
	//Component* AddComponent(Component* _Component);

	template<typename T>
	T* GetComponent() const;

	Component* GetComponent(COMPONENT_TYPE _Type);
	Component* GetComponent(const wstring& _Name);

public:
	virtual void Begin() {}
	virtual void Tick() = 0;		// 해당 Actor 가 매 프레임마다 수행할 내용을 구현 -> 무조건 자식이 구현하게끔 순수 가상 함수 설정
	virtual void FinalTick() final;	// Actor 가 할 일(Tick)을 끝내고 난 뒤의 후속 조치 -> 공통 처리 용으로 건드리지 못 하게 final 선언
	virtual void Render(HDC _dc);	// Actor 가 화면에 그려지는 방식

	virtual void BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) {}
	virtual void Overlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) {}
	virtual void EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider) {}


public:
	Actor();
	~Actor();

	friend class Level;
	friend class TaskMgr;
};

template<typename T>
T* Actor::AddComponent(T* _Component)
{
	// 다른 Actor 의 소유 Component 가 입력된 경우
	assert(!_Component->GetOwner());

	// Actor 가 Component 의 주소를 저장
	m_vecCom.push_back(_Component);

	// Component 가 자신을 소유하게 된 Actor 의 주소를 저장
	_Component->m_Actor = this;

	return _Component;
}

template<typename T>
T* Actor::GetComponent() const
{
	for (size_t i = 0; i < m_vecCom.size(); ++i)
	{
		T* pCom = dynamic_cast<T*>(m_vecCom[i]);

		if (nullptr != pCom)
			return pCom;

	}

	return nullptr;
}
