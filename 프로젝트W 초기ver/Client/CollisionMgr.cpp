#include "pch.h"
#include "CollisionMgr.h"

#include "Actor.h"
#include "Collider.h"

CollisionMgr::CollisionMgr()
	: m_Matrix{}
{
}

CollisionMgr::~CollisionMgr()
{
}

void CollisionMgr::Tick()
{
	for (int Row = 0; Row < (int)ACTOR_TYPE::END; ++Row)
	{
		for (int Col = Row; Col < (int)ACTOR_TYPE::END; ++Col)
		{
			if (m_Matrix[Row] & (1 << Col))
			{
				CollisionByType(Row, Col);
			}
		}
	}

	// 등록된 모든 충돌체를 해제시킨다.
	for (int i = 0; i < (int)ACTOR_TYPE::END; ++i)
	{
		m_Collider[i].clear();
	}
}

void CollisionMgr::CollisionByType(int _Type1, int _Type2)
{
	if (_Type1 != _Type2)
	{
		for (size_t i = 0; i < m_Collider[_Type1].size(); ++i)
		{
			for (size_t j = 0; j < m_Collider[_Type2].size(); ++j)
			{
				CollisionProcess(m_Collider[_Type1][i], m_Collider[_Type2][j]);
			}
		}
	}

	else
	{
		for (size_t i = 0; i < m_Collider[_Type1].size(); ++i)
		{
			for (size_t j = i + 1; j < m_Collider[_Type2].size(); ++j)
			{
				CollisionProcess(m_Collider[_Type1][i], m_Collider[_Type2][j]);
			}
		}
	}	
}

void CollisionMgr::CollisionProcess(Collider* _Left, Collider* _Right)
{
	// _Left, _Right 두 충돌체로 만들수있는 고유한 ID 키값을 생성한다.
	union ID
	{
		struct
		{
			UINT Left;
			UINT Right;
		};

		COL_ID id;
	};

	ID CollisionID = {};
	CollisionID.Left	= _Left->GetID();
	CollisionID.Right	= _Right->GetID();

	// 두 조합 키값으로 이전 프레임에 충돌했었는지 검색한다.
	map<COL_ID, bool>::iterator iter = m_CollisionData.find(CollisionID.id);

	// 정보자체가 등록되어있지 않으면, 새로 등록한다.
	if (iter == m_CollisionData.end())
	{
		m_CollisionData.insert(make_pair(CollisionID.id, false));
		iter = m_CollisionData.find(CollisionID.id);
	}


	// 충돌체를 소유한 Actor 가 Dead 상태인지 아닌지
	bool IsDead = _Left->GetOwner()->IsDead() || _Right->GetOwner()->IsDead();

	// 두 충돌체가 지금 충돌중이다
	if (IsOverlap(_Left, _Right))
	{
		// 이전에는 충돌하지 않았었다.
		if (!iter->second)
		{
			// 두 Actor 중 하나라도 Dead 상태라면 충돌 무효
			if (!IsDead)
			{
				_Left->BeginOverlap(_Right);
				_Right->BeginOverlap(_Left);
			}		
		}

		// 이전에도 충돌중이었다.
		else
		{
			if (IsDead)
			{
				_Left->EndOverlap(_Right);
				_Right->EndOverlap(_Left);
			}
			else
			{
				_Left->Overlap(_Right);
				_Right->Overlap(_Left);
			}			
		}		

		iter->second = true;
	}

	// 두 충돌체가 지금 충돌하고있지 않다.
	else
	{
		// 이전 프레임에는 충돌했었다.
		if (iter->second)
		{
			_Left->EndOverlap(_Right);
			_Right->EndOverlap(_Left);
		}

		iter->second = false;
	}
}

bool CollisionMgr::IsOverlap(Collider* _Left, Collider* _Right)
{
	Vec2 vLeftPos = _Left->GetFinalPos();
	Vec2 vRightPos = _Right->GetFinalPos();

	Vec2 vLeftScale = _Left->GetScale();
	Vec2 vRightScale = _Right->GetScale();

	float XDist = fabsf(vLeftPos.x - vRightPos.x);
	float YDist = fabsf(vLeftPos.y - vRightPos.y);

	if (   XDist <= vLeftScale.x / 2.f + vRightScale.x / 2.f
		&& YDist <= vLeftScale.y / 2.f + vRightScale.y / 2.f)
	{
		return true;
	}

	return false;
}

void CollisionMgr::CollisionCheck(ACTOR_TYPE _Type1, ACTOR_TYPE _Type2, bool _On)
{
	// _Type1, _Type2 중에서 더 낮은 수를 행, 더 큰 수를 열로 생각한다.
	int Row = (int)_Type1;
	int Col = (int)_Type2;

	if (Row > Col)
	{
		Row = (int)_Type2;
		Col = (int)_Type1;
	}

	// | : 비트를 합칠 때
	// & : 특정 자리에 비트값이 있는지 검사
	// &=, ~ : 특정 자리에 비트값을 제거
	// ^= : 특정 자리 비트가 켜져있으면 끄고, 꺼져있으면 킴
	if (_On)
	{
		m_Matrix[Row] |= (1 << Col);
	}
	else
	{
		m_Matrix[Row] &= ~(1 << Col);
	}	
}
