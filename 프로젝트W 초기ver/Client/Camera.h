#pragma once


// 실제 좌표		- 절대좌표, 액터의 좌표, 충돌 연산
// 렌더링 좌표   - 카메라 중심으로부터의 상대좌표, 
//				  렌더링 할때만 카메라로부터의 상대거리를 계산해서 출력할 픽셀위치를 계산한다.

enum class POST_PROCESS
{
	FADE_IN,
	FADE_OUT,
};


struct tPostProcess
{
	POST_PROCESS	Type;
	float			Time;
	float			Duration;	
};



class Texture;
class Camera
{
	SINGLE(Camera);
private:
	Vec2		m_LookAt;	// 카메라가 바라보는 중앙지점(실제 좌표)
	Vec2		m_Offset;

	// 카메라 진동효과
	float		m_Frequency;		// 주파수, 초당 진동 횟수
	float		m_Amplitude;		// 진폭, 중심 m_LookAt 에서 움직이는 최대 범위
	float		m_OscillTime;		// 진동 시간
	float		m_OsccilCurTime;	// 진행중인 시간
	float		m_OsccilDir;		// 진행 방향 1 / -1
	bool		m_bOscilliation;	// 진동효과 On / Off

	Vec2		m_Diff;				// LookAt 지점에서 해상도 중심을 뺀 차이값
	float		m_Speed;	


	list<tPostProcess> m_PostProcessList;	// 후처리 효과 작업 리스트
	Texture*		   m_BlackTex;			// 검은색 텍스쳐

public:
	void SetLookAt(Vec2 _vLookAt) { m_LookAt = _vLookAt; }
	Vec2 GetLookAt() { return m_LookAt + m_Offset; }
	Vec2 GetRenderPos(Vec2 _RealPos) { return _RealPos - m_Diff; }
	Vec2 GetRealPos(Vec2 _RenderPos) { return _RenderPos + m_Diff; }
	
	void SetOscillation(float _Frequency, float _Amplitude, float _Time)
	{
		m_bOscilliation = true;
		m_Frequency = _Frequency;
		m_Amplitude = _Amplitude;
		m_OscillTime = _Time;
		m_OsccilCurTime = 0.f;
		m_OsccilDir = 1.f;
	}

	void AddFadeIn(float _Duration) { m_PostProcessList.push_back(tPostProcess{ POST_PROCESS::FADE_IN, 0.f, _Duration }); }
	void AddFadeOut(float _Duration) { m_PostProcessList.push_back(tPostProcess{ POST_PROCESS::FADE_OUT, 0.f, _Duration }); }

public:
	void Tick();
	void Render(HDC _dc);

private:
	void Oscilliation();
};

