#pragma once

// Vector (크기, 방향)
// Scalar (크기)
struct Vec2
{
	float x;
	float y;

public:
	float Dot(Vec2 _Other)
	{
		// 각 축별 성분의 곱의 합 == 각 벡터의 길이 x 길이 x 두벡터가 이루는 Cos
		return(x * _Other.x) + (y * _Other.y);
	}

	float Distance(Vec2 _Other) { return sqrtf((x - _Other.x) * (x - _Other.x) + (y - _Other.y) * (y - _Other.y)); }
	float Length() { return sqrtf(x * x + y * y); }
	Vec2& Normalize() { float Len = Length(); x /= Len; y /= Len; return *this; }

	bool IsZero() { return (x == 0.f && y == 0.f); }


	Vec2 operator + (Vec2 _Other) { return Vec2(x + _Other.x, y + _Other.y); }
	Vec2 operator - (Vec2 _Other) { return Vec2(x - _Other.x, y - _Other.y); }
	Vec2 operator * (Vec2 _Other) { return Vec2(x * _Other.x, y * _Other.y); }
	Vec2 operator / (Vec2 _Other) { return Vec2(x / _Other.x, y / _Other.y); }

	Vec2 operator + (float _f) { return Vec2(x + _f, y + _f); }
	Vec2 operator - (float _f) { return Vec2(x - _f, y - _f); }
	Vec2 operator * (float _f) { return Vec2(x * _f, y * _f); }
	Vec2 operator / (float _f) { return Vec2(x / _f, y / _f); }

	void operator += (Vec2 _Other) { x += _Other.x, y += _Other.y; }
	void operator -= (Vec2 _Other) { x -= _Other.x, y -= _Other.y; }
	void operator *= (Vec2 _Other) { x *= _Other.x, y *= _Other.y; }
	void operator /= (Vec2 _Other) { x /= _Other.x, y /= _Other.y; }

	void operator += (float _f) { x += _f, y += _f; }
	void operator -= (float _f) { x -= _f, y -= _f; }
	void operator *= (float _f) { x *= _f, y *= _f; }
	void operator /= (float _f) { x /= _f, y /= _f; }

	bool operator ==(Vec2 _Other) { return (x == _Other.x) && (y == _Other.y); }
	bool operator != (Vec2 _Other) { return (x != _Other.x) || (y != _Other.y); }

	Vec2& operator = (POINT _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;
		return *this;
	}


	Vec2 operator -() { return Vec2(-x, -y); }

	Vec2()
		: x(0.f)
		, y(0.f)
	{}

	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}

	Vec2(int _x, int _y)
		: x(float(_x))
		, y(float(_y))
	{}

	Vec2(UINT _x, UINT _y)
		: x(float(_x))
		, y(float(_y))
	{
	}

	Vec2(POINT _pt)
		: x(float(_pt.x))
		, y(float(_pt.y))
	{}
};


struct tDebugShape
{
	DEBUG_SHAPE		Shape;	// 디버그 렌더링 모양
	Vec2			Center;	// 중심 좌표
	Vec2			Scale;	// 가로 세로 길이

	PEN_TYPE		Pen;	// 원하는 펜

	float			Age;	// 모양이 그려진 시간
	float			Life;	// 모양 유지 시간
};


// 작업, 이벤트 단위
struct tTask
{
	TASK_TYPE	Type;
	DWORD_PTR	Param0; 
	DWORD_PTR	Param1;
};
