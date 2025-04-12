#include "pch.h"
#include "Camera.h"

#include "Engine.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

#include "AssetMgr.h"
#include "Texture.h"

Camera::Camera()
	: m_Speed(500.f)
	, m_BlackTex(nullptr)
{
	Vec2 Resol = Engine::GetInst()->GetResolution();
	m_BlackTex = AssetMgr::GetInst()->CreateTexture( L"CameraTex"
													, (UINT)Resol.x, (UINT)Resol.y);
}

Camera::~Camera()
{
}

void Camera::Tick()
{
	if (KEY_PRESSED(KEY::W))
		m_LookAt.y -= m_Speed* DT;
	if (KEY_PRESSED(KEY::S))
		m_LookAt.y += m_Speed * DT;
	if (KEY_PRESSED(KEY::A))
		m_LookAt.x -= m_Speed * DT;
	if (KEY_PRESSED(KEY::D))
		m_LookAt.x += m_Speed * DT;

	// 카메라 진동효과
	Oscilliation();

	// 카메라가 바라보는 위치와, 실제 해상도 중심까지의 차이값
	Vec2 vResolution = Engine::GetInst()->GetResolution();
	m_Diff = (m_LookAt + m_Offset) - (vResolution / 2.f);
}

void Camera::Render(HDC _dc)
{
	if (m_PostProcessList.empty())
		return;

	tPostProcess& post = m_PostProcessList.front();
	
	// 작업 진행률
	float ProcessRatio = (post.Time / post.Duration);
	float Alpha = 0.f;

	if (POST_PROCESS::FADE_IN == post.Type)
	{
		Alpha = 1.f - ProcessRatio;
	}
	else if (POST_PROCESS::FADE_OUT == post.Type)
	{
		Alpha = ProcessRatio;
	}

	// 0 ~ 1 을 넘어갈 수 없게 보정하기
	Saturate(Alpha);

	BLENDFUNCTION blend = {};
	blend.AlphaFormat = 0; // 알파값을 계수로 사용
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = (int)(Alpha * 255.f);

	Vec2 vResol = Engine::GetInst()->GetResolution();

	AlphaBlend(_dc
		, 0, 0
		, (int)vResol.x, (int)vResol.y
		, m_BlackTex->GetDC()
		, 0, 0
		, (int)m_BlackTex->GetWidth(), (int)m_BlackTex->GetHeight()
		, blend);

	// 효과 시간이 만료되었으면
	post.Time += DT;
	if (post.Duration <= post.Time)
	{
		m_PostProcessList.pop_front();
	}
}

void Camera::Oscilliation()
{
	if (!m_bOscilliation)
		return;

	m_OsccilCurTime += DT;
	if (m_OscillTime < m_OsccilCurTime)
	{
		m_Offset = Vec2(0.f, 0.f);
		m_bOscilliation = false;
		return;
	}

	float Speed = m_Amplitude * 4.f * m_Frequency;

	// 1프레임 카메라 이동량 
	m_Offset.y += Speed * DT * m_OsccilDir;

	// 진폭을 넘어서면 방향을 전환해야함
	if (m_Amplitude < fabsf(m_Offset.y))
	{
		// 방향 전환하기
		m_OsccilDir *= -1.f;

		// 초과량 구하기
		float fOver = fabsf(m_Offset.y) - m_Amplitude;

		// 초과량만큼 안쪽으로 넣어주기
		m_Offset.y = m_OsccilDir * fOver * 2.f;
	}
}
