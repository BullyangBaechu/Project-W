#pragma once

class Collider;

typedef long long COL_ID;


class CollisionMgr
{
	SINGLE(CollisionMgr)
private:
	// 충돌 체크 매트릭스
	UINT				m_Matrix[(UINT)ACTOR_TYPE::END];

	// 각 그룹 소속 오브젝트들의 충돌체
	vector<Collider*>	m_Collider[(UINT)ACTOR_TYPE::END];

	// 각 충돌체 조합별로 이전프레임 충돌정보를 기록
	map<COL_ID, bool>	m_CollisionData;


public:	
	void CollisionCheck(ACTOR_TYPE _Type1, ACTOR_TYPE _Type2, bool _On);
	void CollisionCheckClear() { memset(m_Matrix, 0, sizeof(UINT) * 32); }
	void RegisterCollider(ACTOR_TYPE _Type, Collider* _Collider) { m_Collider[(UINT)_Type].push_back(_Collider); }

public:
	void Tick();

private:
	void CollisionByType(int _Type1, int _Type2);
	void CollisionProcess(Collider* _Left, Collider* _Right);
	bool IsOverlap(Collider* _Left, Collider* _Right);
};

