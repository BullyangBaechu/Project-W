#include "pch.h"
#include "Player.h"

#include "TimeMgr.h"
#include "KeyMgr.h"

#include "RenderMgr.h"
#include "AssetMgr.h"
#include "Texture.h"

#include "Collider.h"
#include "FlipbookPlayer.h"
#include "Flipbook.h"
#include "Sprite.h"
#include "RigidBody.h"

#include "GuidedMissile.h"
#include "TaskMgr.h"

#include "Engine.h"
#include "Camera.h"
#include "Slowzone.h"


Player::Player()
	: m_Speed(500.f)			// 초당 500 픽셀 이동
	, m_SpeedMultiplier(1.f)	// 디폴트 값 (감속 장판 충돌 시 감소)
	, m_Texture(nullptr)
	, m_HurtBox(nullptr)
	, m_FBPlayer(nullptr)
	, m_RigidBody(nullptr)
	, m_AttackTimer(0.f)
	, m_AttackCollider(nullptr)
	, m_Dmg(1)
	, m_Level(1)
	, m_exp(0.f)
	, m_MaxHP(1)
	//, m_BlockLeft(false)
	//, m_BlockRight(false)
{
	// Player의 Collider 컴포넌트개 추가 및 값 설정	
	m_HurtBox = AddComponent(new Collider);
	m_HurtBox->SetName(L"HurtBox");
	m_HurtBox->SetOffset(Vec2(-30.f, 0.f));
	m_HurtBox->SetScale(Vec2(120.f, 192.f));

	


	//m_Texture = AssetMgr::GetInst()->LoadTexture(L"PlayerImage", L"Texture\\Fighter.bmp");

	// FlipbookPlayer 컴포넌트 추가
	m_FBPlayer = AddComponent(new FlipbookPlayer);		

	// CyborgRun 하나만 등록
	Flipbook* pFB = AssetMgr::GetInst()->LoadFlipbook(L"CyborgRun", L"Flipbook\\CyborgRun.flip");
	m_FBPlayer->AddFlipbook(pFB, (int)PLAYER_ANIM::CYBORG_RUN);

	// CyborgAttack 등록
	Flipbook* pFBAtk = AssetMgr::GetInst()->LoadFlipbook(L"CyborgAtk4x", L"Flipbook\\CyborgAtk4x.flip");
	m_FBPlayer->AddFlipbook(pFBAtk, (int)PLAYER_ANIM::CYBORG_ATTACK);

	// CyborgGuard 등록
	Flipbook* pFBGuard = AssetMgr::GetInst()->LoadFlipbook(L"CyborgGuard4x", L"Flipbook\\CyborgGuard4x.flip");
	m_FBPlayer->AddFlipbook(pFBGuard, (int)PLAYER_ANIM::CYBORG_GUARD);


	// 수업때 쓴 임시 데이터들 (맨 마지막에 삭제)
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_IDLE_DOWN" , L"Flipbook\\PLAYER_IDLE_DOWN.flip" ), (int)PLAYER_ANIM::IDLE_DOWN);
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_IDLE_LEFT" , L"Flipbook\\PLAYER_IDLE_LEFT.flip" ), (int)PLAYER_ANIM::IDLE_LEFT );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_IDLE_UP"   , L"Flipbook\\PLAYER_IDLE_UP.flip"   ), (int)PLAYER_ANIM::IDLE_UP   );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_IDLE_RIGHT", L"Flipbook\\PLAYER_IDLE_RIGHT.flip"), (int)PLAYER_ANIM::IDLE_RIGHT);

	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_MOVE_DOWN" , L"Flipbook\\PLAYER_MOVE_DOWN.flip" ), (int)PLAYER_ANIM::MOVE_DOWN );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_MOVE_LEFT" , L"Flipbook\\PLAYER_MOVE_LEFT.flip" ), (int)PLAYER_ANIM::MOVE_LEFT );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_MOVE_UP"   , L"Flipbook\\PLAYER_MOVE_UP.flip"   ), (int)PLAYER_ANIM::MOVE_UP   );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_MOVE_RIGHT", L"Flipbook\\PLAYER_MOVE_RIGHT.flip"), (int)PLAYER_ANIM::MOVE_RIGHT);

	// m_FBPlayer->Play((int)PLAYER_ANIM::IDLE_DOWN, 5.f);
	// 항상 오른쪽으로 달리게끔
	//m_FBPlayer->Play((int)PLAYER_ANIM::MOVE_RIGHT, 15.f, 0, 0);

	// 항상 오른쪽으로 달리게끔
	m_FBPlayer->Play((int)PLAYER_ANIM::CYBORG_RUN, 12.f, 0, 0);

	Flipbook* pFlipbook = m_FBPlayer->GetFlipbook((int)PLAYER_ANIM::IDLE_RIGHT);
	Sprite* pSprite = pFlipbook->GetSprite(1);
	pSprite->SetOffset(Vec2(1.f, 0.f));

	// RigidBody
	m_RigidBody = AddComponent(new RigidBody);
	m_RigidBody->SetRigidBodyMode(RIGIDBODY_MODE::BELTSCROLL);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetInitSpeed(200.f);
	m_RigidBody->SetMaxSpeed(700.f);
	m_RigidBody->SetFriction(500.f);
	m_RigidBody->SetJumpSpeed(500.f);
}

Player::~Player()
{
	
}

void Player::Tick()
{	

	PlayerGetExp();

	// 충돌 시 사용할 이전 프레임 위치
	m_PrevPos = GetPos();

	// 플레이어가 타겟 좌표까지 자동으로 이동하게끔
	// 1. 카메라 기준으로 좌표를 구하기 
	Vec2 vPos = GetPos();
	float RelativePosX = Camera::GetInst()->GetLookAt().x;

	// 2. 타겟 좌표 = 카메라(상대적) 좌표 + 절대 좌표
	const float TargetX = RelativePosX + 500.f; 
	const float RecoverFactor = 10.f; // 보간 회복 속도 조절용

	// 3. 타겟 좌표와 캐릭터 좌표의 x축 차이
	float deltaX = TargetX - vPos.x;

	// 4. 타겟 좌표까지 도달하다록 force 부여
	if (fabs(deltaX) < 1.f)
	{
		// 기준 위치에 거의 도달했으면 정지
		m_RigidBody->AddForce(Vec2(0.f, 0.f));
	}
	else
	{
		// 기준 위치로 돌아가기 위해 보간 힘 적용
		

		float recoverSpeed = deltaX * RecoverFactor * m_SpeedMultiplier;

		m_RigidBody->AddForce(Vec2(recoverSpeed, 0.f));
	}
	


	// -  Player가 오른쪽 화면 밖으로 못 나가게끔 처리 - 후에 필요없으면 삭제
	//
	// 1. 카메라의 오른쪽 경계 좌표
	Vec2 CamPos = Camera::GetInst()->GetLookAt();
	Vec2 Res = Engine::GetInst()->GetResolution();
	float RightLimit = CamPos.x + Res.x / 2.0f;

	// 2. Player 객체의 중심이 아닌, 오른쪽 경계로 계산 처리
	float PlayerHalfWidth = GetScale().x / 2.0f;

	// 3. 오른쪽 경계 밖으로 나가면 위치 재지정
	if (vPos.x + PlayerHalfWidth > RightLimit)
		vPos.x = RightLimit - PlayerHalfWidth;

	//ChangeFlipbook();



	if (KEY_TAP(KEY::SPACE))
	{
		//Shoot();
		m_RigidBody->Jump();
		//Camera::GetInst()->AddFadeOut(0.4f);
		//Camera::GetInst()->AddFadeIn(0.4f);
	}
	//m_FBPlayer->FinalTick();
	

	// 플레이어 공격 판정
	if (KEY_TAP(KEY::CTRL))
		PlayerAttack();
	if (m_AttackCollider && m_AttackCollider->IsEnable())
	{
		m_AttackTimer -= DT;

		if (m_AttackTimer <= 0.f)
		{
			m_AttackCollider->SetEnable(false);
			m_AttackCollider = nullptr;
			m_FBPlayer->Play((int)PLAYER_ANIM::CYBORG_RUN, 12.f, 0, 0);
		}
	}


	// 플레이어 가드 판정
	if (KEY_TAP(KEY::LSHFT))
		PlayerGuard();
	if (m_GuardCollider && m_GuardCollider->IsEnable())
	{
		//CollisionMgr::GetInst()->RegisterCollider(ACTOR_TYPE::PLAYER, m_GuardCollider);
		m_GuardTimer -= DT;
		
		if (m_GuardTimer <= 0.f)
		{
			m_GuardCollider->SetEnable(false);
			m_GuardCollider = nullptr;
			m_FBPlayer->Play((int)PLAYER_ANIM::CYBORG_RUN, 12.f, 0, 0);
		}
	}

	if (m_IsInSlowzone)
		OutputDebugString(L"[슬로우존 상태] 감속 중입니다\n");
	else
		OutputDebugString(L"[정상] 일반 속도\n");

	// 감속 적용
	Vec2 vel = m_RigidBody->GetVelocity();
	vel.x *= m_SpeedMultiplier;
	m_RigidBody->SetVelocity(vel);

	SetPos(vPos);
}

// 이동 거리 기반 레벨 업
void Player::PlayerGetExp()
{
	Vec2 velocity = m_RigidBody->GetVelocity();

	if (velocity.x > 0.f)
	{
		float distance = velocity.x * DT;
		m_exp += distance;
	}

	static float expThresholds[5] = { 0.f, 1000.f, 3000.f, 6000.f, 10000.f };

	while (m_Level < 5 && m_exp >= expThresholds[m_Level])
	{
		++m_Level;
		// 필요하면 레벨업 이펙트/사운드 추가
	}

}
// 안 쓰면 삭제 예정
/*
void Player::Shoot()
{
	// 미사일 객체 생성 및 위치, 크기 설정
	GuidedMissile* pMissile = new GuidedMissile;
	pMissile->SetName(L"Missile");
	pMissile->SetPos(GetPos() + Vec2(0.f, -30.f));
	pMissile->SetScale(Vec2(20.f, 20.f));
	pMissile->SetDegree(90.f);
	pMissile->SetSpeed(300.f);

	Vec2 velociy(0.f, -1.f);
	velociy.Normalize();
	velociy *= 300.f;
	pMissile->SetVelocity(velociy);
	pMissile->SetRotateSpeed(100.f);
	pMissile->SetTargetActorType(ACTOR_TYPE::ENERMY);

	CreateActor(ACTOR_TYPE::PLAYER_PROJECTILE, pMissile);

	// 카메라 진동효과 요청
	Camera::GetInst()->SetOscillation(8.f, 20.f, 0.2f);
}
*/
void Player::PlayerAttack()
{
	// 아직 남아 있는 공격 판정이 있다면 → 새로 생성 막기
	if (m_AttackCollider && m_AttackCollider->IsEnable())
		return;

	// 공격 프레임 이전 플립북 기억
	//m_PrevAnimID = m_FBPlayer.ge

	// 공격 플립북 재생 (임시로 왼쪽 Idle 넣기)
	//m_FBPlayer->Play((int)PLAYER_ANIM::IDLE_LEFT, 10.f, 0, 1);
	m_FBPlayer->Play((int)PLAYER_ANIM::CYBORG_ATTACK, 12.f, 0, 1);

	// 공격 콜라이더 생성 
	
	m_AttackCollider = AddComponent(new Collider);
	
	m_AttackCollider->SetName(L"AttackBox");
	m_AttackCollider->SetOffset(Vec2(0.f, 20.f));
	m_AttackCollider->SetScale(Vec2(80.f, 30.f));
	m_AttackCollider->SetEnable(true);

	m_AttackTimer = 0.3f;
}

void Player::PlayerGuard()
{
	// 방어 플립북 재생
	m_FBPlayer->Play((int)PLAYER_ANIM::CYBORG_GUARD, 12.f, 0, 1);
	
	
	m_GuardCollider = AddComponent(new Collider);

	m_GuardCollider->SetName(L"GuardBox");
	m_GuardCollider->SetOffset(Vec2(-40.f, -100.f));
	m_GuardCollider->SetScale(Vec2(200.f,200.f));
	//m_GuardCollider->SetColliderMode(ColliderType::Circle);
	m_GuardCollider->SetEnable(true);

	//CollisionMgr::GetInst()->RegisterCollider(ACTOR_TYPE::PLAYER, m_GuardCollider);

	m_GuardTimer = 0.5f;

}

// 캐릭터 자동이동으로 변경된 이 시점에서는 안 쓸수도
void Player::ChangeFlipbook()
{
	// 항상 오른쪽으로 달리게끔
	m_FBPlayer->Play((int)PLAYER_ANIM::CYBORG_RUN, 12.f, 0, 0);

	/*
	if (KEY_TAP(KEY::LEFT))
		m_FBPlayer->Play((int)PLAYER_ANIM::MOVE_LEFT, 15.f);
	if (KEY_TAP(KEY::RIGHT))
		m_FBPlayer->Play((int)PLAYER_ANIM::MOVE_RIGHT, 15.f);

	if (KEY_RELEASED(KEY::LEFT))
		m_FBPlayer->Play((int)PLAYER_ANIM::IDLE_LEFT, 5.f);
	if (KEY_RELEASED(KEY::RIGHT))
		m_FBPlayer->Play((int)PLAYER_ANIM::IDLE_RIGHT, 5.f);
	*/
}


void Player::Render(HDC _dc)
{	
	m_FBPlayer->Render(_dc);

	// 임시 UI
	// ====== HUD: 레벨/경험치 표시 ======

	// 문자열 만들기
	wchar_t szText[128] = {};
	swprintf_s(szText, L"Lv.%d  Exp: %.0f", m_Level, m_exp);

	// 출력 위치 (좌상단 기준)
	Vec2 screenPos = Vec2(10.f, 10.f);

	// SetBkMode를 통해 배경 투명하게
	SetBkMode(_dc, TRANSPARENT);

	// 검은색 글자로 출력
	SetTextColor(_dc, RGB(0, 0, 0));
	TextOut(_dc, (int)screenPos.x, (int)screenPos.y, szText, (int)wcslen(szText));


	Vec2 velocity = m_RigidBody->GetVelocity();

	wchar_t szSpeed[128] = {};
	swprintf_s(szSpeed, L"Speed: %.2f", velocity.x);
	TextOut(_dc, 10, 30, szSpeed, (int)wcslen(szSpeed));


}

void Player::BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
	

	if (_OtherActor->GetName() == L"Ground")
	{
		m_RigidBody->SetGround(true);
	}

	else if (_OtherActor->GetActorType() == ACTOR_TYPE::BRICK)
	{
		m_RigidBody->CollisionResponse(this, _OtherActor);
		
		
		/*
		Vec2 MyPos = GetPos();
		Vec2 OtherPos = _OtherActor->GetPos();

		Vec2 MyScale = GetScale();
		Vec2 OtherScale = _OtherActor->GetScale();

		Vec2 delta = OtherPos - MyPos;

		// X 축 겹침 길이 vs Y 축 겹침 길이를 통해 X축에서 충돌난건지 Y축에서 충돌난건지 check
		float overlapX = (MyScale.x + OtherScale.x) / 2.f - abs(delta.x);
		float overlapY = (MyScale.y + OtherScale.y) / 2.f - abs(delta.y);

		// X 축에서 충돌났을때만 좌우 Force를 막아야하니까
		/*
		if (overlapX < overlapY)
		{
			if (delta.x > 0.f)
				m_BlockRight = true;
			else
				m_BlockLeft = true;
		}
		/* 스펙 변경에 따라 y축 충돌 여부가 없어짐
		else
		{
			if (delta.y > 0.f)
			{
				// 수직 방향 충돌이며, 내가 위에 있음 → 착지로 간주
				m_RigidBody->SetGround(true);
				// 단, 이때 충돌 박스가 충분히 안쪽에 들어왔는지도 확인하면 더 좋음
			}
		}
		*/

		Vec2 velocity = m_RigidBody->GetVelocity();
		//velocity.y 
		m_RigidBody->SetVelocity(velocity);
		

	
	}

	else if (_OtherActor->GetName() == L"Slowzone")
	{
		Slowzone* slowzone = (Slowzone*)_OtherActor;
		m_SpeedMultiplier = 0.9f;
		//m_SpeedMultiplier = slowzone->GetSlowRatio();

		m_IsInSlowzone = true;

	}
}

void Player::Overlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
	if (_OtherActor->GetName() == L"Slowzone")
	{
		Slowzone* slowzone = (Slowzone*)_OtherActor;
		m_SpeedMultiplier = 0.95f;
		//m_SpeedMultiplier = slowzone->GetSlowRatio();

		m_IsInSlowzone = true;
	}
}

void Player::EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{	
	if (_OtherActor->GetName() == L"Ground")
	{
		//m_RigidBody->SetGround(false);
	}
	else if (_OtherActor->GetActorType() == ACTOR_TYPE::BRICK)
	{
		//m_BlockLeft = false;
		//m_BlockRight = false;
	}
	else if (_OtherActor->GetName() == L"Slowzone")
	{
		m_SpeedMultiplier = 1.f;
		m_IsInSlowzone = false;
	}
}


bool Player::CamCheck()
{
	Flipbook* pFlipbook = m_FBPlayer->GetCurFlipbook();
	if (nullptr == pFlipbook)
		return false;

	int Idx = m_FBPlayer->GetCurSpriteIdx();	
	Sprite* pSprite = pFlipbook->GetSprite(Idx);

	Vec2 vCenter = GetPos() + pSprite->GetOffset();
	Vec2 vScale = pSprite->GetSlice();

	Vec2 vResol = Engine::GetInst()->GetResolution();
	Vec2 vLookAt = Camera::GetInst()->GetLookAt();

	if ((vLookAt.x - (vResol.x / 2.f) < vCenter.x + vScale.x / 2.f && vCenter.x - vScale.x / 2.f < vLookAt.x + (vResol.x / 2.f))
		&& (vLookAt.y - (vResol.y / 2.f) < vCenter.y + vScale.y / 2.f && vCenter.y - vScale.y / 2.f < vLookAt.y + (vResol.y / 2.f)))
	{
		return true;
	}

	return false;
}

