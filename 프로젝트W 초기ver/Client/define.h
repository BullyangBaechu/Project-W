#pragma once


#define SINGLE(Type) public:\
						static Type* GetInst()\
						{\
							static Type engine;\
							return &engine;\
						}\
					 private:\
						Type();\
						Type(const Type& _E) = delete;\
						Type& operator=(const Type& _Other) = delete;\
					 public:\
						~Type();


#define DT	TimeMgr::GetInst()->GetDeltaTime()

#define KEY_CHECK(Key, State) (State == KeyMgr::GetInst()->GetKeyState(Key))
#define KEY_TAP(Key)		KEY_CHECK(Key, KEY_STATE::TAP)
#define KEY_PRESSED(Key)	KEY_CHECK(Key, KEY_STATE::PRESSED)
#define KEY_RELEASED(Key)	KEY_CHECK(Key, KEY_STATE::RELEASED)
#define KEY_NONE(Key)		KEY_CHECK(Key, KEY_STATE::NONE)

#define SEL_PEN(dc, PenType) SelectGDI selectPen(dc, RenderMgr::GetInst()->GetPen(PenType))
#define SEL_BRUSH(dc, BrushType) SelectGDI selectBrush(dc, RenderMgr::GetInst()->GetBrush(BrushType))

#define GET_SET(Type, ParameterName) Type Get##ParameterName () { return m_##ParameterName; }\
									 void Set##ParameterName (Type _##ParameterName) { m_##ParameterName = _##ParameterName; }


#define PI 3.14159265f

// 해상도 결정
#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 900
#define GROUND_Y (WINDOW_HEIGHT - 100.f)

