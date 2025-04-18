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



Player::Player()
	: m_Speed(300.f) // 초당 100 픽셀 이동	
	, m_Texture(nullptr)
	, m_HurtBox(nullptr)
	, m_FBPlayer(nullptr)
	, m_RigidBody(nullptr)
{
	// Collider 컴포넌트 2개 추가 및 값 설정	
	m_HurtBox = AddComponent(new Collider);
	m_HurtBox->SetName(L"HurtBox");
	m_HurtBox->SetOffset(Vec2(-5.f, 14.f));
	m_HurtBox->SetScale(Vec2(40.f, 100.f));

	m_Texture = AssetMgr::GetInst()->LoadTexture(L"PlayerImage", L"Texture\\Fighter.bmp");

	// FlipbookPlayer 컴포넌트 추가
	m_FBPlayer = AddComponent(new FlipbookPlayer);		

	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_IDLE_DOWN" , L"Flipbook\\PLAYER_IDLE_DOWN.flip" ), (int)PLAYER_ANIM::IDLE_DOWN);
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_IDLE_LEFT" , L"Flipbook\\PLAYER_IDLE_LEFT.flip" ), (int)PLAYER_ANIM::IDLE_LEFT );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_IDLE_UP"   , L"Flipbook\\PLAYER_IDLE_UP.flip"   ), (int)PLAYER_ANIM::IDLE_UP   );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_IDLE_RIGHT", L"Flipbook\\PLAYER_IDLE_RIGHT.flip"), (int)PLAYER_ANIM::IDLE_RIGHT);

	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_MOVE_DOWN" , L"Flipbook\\PLAYER_MOVE_DOWN.flip" ), (int)PLAYER_ANIM::MOVE_DOWN );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_MOVE_LEFT" , L"Flipbook\\PLAYER_MOVE_LEFT.flip" ), (int)PLAYER_ANIM::MOVE_LEFT );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_MOVE_UP"   , L"Flipbook\\PLAYER_MOVE_UP.flip"   ), (int)PLAYER_ANIM::MOVE_UP   );
	m_FBPlayer->AddFlipbook(AssetMgr::GetInst()->LoadFlipbook(L"PLAYER_MOVE_RIGHT", L"Flipbook\\PLAYER_MOVE_RIGHT.flip"), (int)PLAYER_ANIM::MOVE_RIGHT);

	m_FBPlayer->Play((int)PLAYER_ANIM::IDLE_DOWN, 5.f);

	Flipbook* pFlipbook = m_FBPlayer->GetFlipbook((int)PLAYER_ANIM::IDLE_RIGHT);
	Sprite* pSprite = pFlipbook->GetSprite(1);
	pSprite->SetOffset(Vec2(1.f, 0.f));

	// RigidBody
	m_RigidBody = AddComponent(new RigidBody);
	m_RigidBody->SetRigidBodyMode(RIGIDBODY_MODE::BELTSCROLL);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetInitSpeed(50.f);
	m_RigidBody->SetMaxSpeed(500.f);
	m_RigidBody->SetFriction(700.f);
	m_RigidBody->SetJumpSpeed(1000.f);
}

Player::~Player()
{
	
}

void Player::Tick()
{	
	Vec2 vPos = GetPos();

	if (KEY_PRESSED(KEY::LEFT))
		m_RigidBody->AddForce(Vec2(-200.f, 0.f));
	if (KEY_PRESSED(KEY::RIGHT))
		m_RigidBody->AddForce(Vec2(200.f, 0.f));
	
	// -  Player가 오른쪽 화면 밖으로 못 나가게끔 처리 -
	// 1. 카메라의 오른쪽 경계 좌표
	Vec2 CamPos = Camera::GetInst()->GetLookAt();
	Vec2 Res = Engine::GetInst()->GetResolution();
	float RightLimit = CamPos.x + Res.x / 2.0f;

	// 2. Player 객체의 중심이 아닌, 오른쪽 경계로 계산 처리
	float PlayerHalfWidth = GetScale().x / 2.0f;

	// 3. 오른쪽 경계 밖으로 나가면 위치 재지정
	if (vPos.x + PlayerHalfWidth > RightLimit)
		vPos.x = RightLimit - PlayerHalfWidth;

	ChangeFlipbook();

	if (KEY_TAP(KEY::SPACE))
	{
		//Shoot();
		m_RigidBody->Jump();
		//Camera::GetInst()->AddFadeOut(0.4f);
		//Camera::GetInst()->AddFadeIn(0.4f);
	}

	SetPos(vPos);
}

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

void Player::ChangeFlipbook()
{
	if (KEY_TAP(KEY::LEFT))
		m_FBPlayer->Play((int)PLAYER_ANIM::MOVE_LEFT, 15.f);
	if (KEY_TAP(KEY::RIGHT))
		m_FBPlayer->Play((int)PLAYER_ANIM::MOVE_RIGHT, 15.f);

	if (KEY_RELEASED(KEY::LEFT))
		m_FBPlayer->Play((int)PLAYER_ANIM::IDLE_LEFT, 5.f);
	if (KEY_RELEASED(KEY::RIGHT))
		m_FBPlayer->Play((int)PLAYER_ANIM::IDLE_RIGHT, 5.f);
}


void Player::Render(HDC _dc)
{	
	m_FBPlayer->Render(_dc);
}

void Player::BeginOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{
	if (_OtherActor->GetName() == L"Monster")
	{
		int a = 0;
	}

	else if (_OtherActor->GetName() == L"Ground")
	{
		m_RigidBody->SetGround(true);
	}
}

void Player::EndOverlap(Collider* _Own, Actor* _OtherActor, Collider* _OtherCollider)
{	
	if (_OtherActor->GetName() == L"Ground")
	{
		m_RigidBody->SetGround(false);
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

