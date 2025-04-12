#include "pch.h"
#include "func.h"

#include "RenderMgr.h"
void DrawDebugRect(Vec2 _Center, Vec2 _Scale, PEN_TYPE _Pen, float _Life)
{
	tDebugShape shape = {};

	shape.Shape = DEBUG_SHAPE::RECT;
	shape.Center = _Center;
	shape.Scale = _Scale;
	shape.Age = 0.f;
	shape.Life = _Life;
	shape.Pen = _Pen;

	RenderMgr::GetInst()->AddDebugShape(shape);
}

void DrawDebugCircle(Vec2 _Center, float _Radius, PEN_TYPE _Pen, float _Life)
{
	tDebugShape shape = {};

	shape.Shape = DEBUG_SHAPE::CIRCLE;
	shape.Center = _Center;
	shape.Scale = Vec2(_Radius * 2.f, _Radius * 2.f);
	shape.Age = 0.f;
	shape.Life = _Life;
	shape.Pen = _Pen;

	RenderMgr::GetInst()->AddDebugShape(shape);
}

void DrawDebugLine(Vec2 _Start, Vec2 _End, PEN_TYPE _Pen, float _Life)
{
	tDebugShape shape = {};

	shape.Shape = DEBUG_SHAPE::LINE;
	shape.Center = _Start;
	shape.Scale = _End;
	shape.Age = 0.f;
	shape.Life = _Life;
	shape.Pen = _Pen;

	RenderMgr::GetInst()->AddDebugShape(shape);
}

#include "TaskMgr.h"
void CreateActor(ACTOR_TYPE _Type, class Actor* _Actor)
{
	// Task 처리
	tTask task = {};

	task.Type = TASK_TYPE::CREATE_ACTOR;
	task.Param0 = (DWORD_PTR)_Actor;
	task.Param1 = (DWORD_PTR)_Type;

	TaskMgr::GetInst()->AddTask(task);
}

void ChangeLevel(LEVEL_TYPE _Next)
{
	// Task 처리
	tTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param0 = (DWORD_PTR)_Next;

	TaskMgr::GetInst()->AddTask(task);
}

#include "Actor.h"
bool IsValid(Actor*& _Actor)
{
	if (nullptr == _Actor)
		return false;

	if (_Actor->IsDead())
	{
		_Actor = nullptr;
		return false;
	}

	return true;
}

Vec2 Rotate(Vec2 _Dir, float _Angle)
{
	Vec2 vRotate;

	vRotate.x = cosf(_Angle) * _Dir.x - sinf(_Angle) * _Dir.y;
	vRotate.y = sinf(_Angle) * _Dir.x + cosf(_Angle) * _Dir.y;	

	return vRotate;
}

void StringSave(const wstring& _String, FILE* _File)
{	
	// 문자열의 길이 저장
	int Len = (int)_String.length();
	fwrite(&Len, sizeof(int), 1, _File);

	// 실제 문자열 저장
	fwrite(_String.c_str(), sizeof(wchar_t), Len, _File);
}

void StringLoad(wstring& _String, FILE* _File)
{
	// 문자열 길이 불러오기
	int Len = 0;
	fread(&Len, sizeof(int), 1, _File);

	// 문자열 불러오기
	wchar_t szBuffer[255] = {};
	fread(szBuffer, sizeof(wchar_t), (size_t)Len, _File);
	_String = szBuffer;
}

#include "AssetMgr.h"
#include "Asset.h"
void SaveAsset(Asset* _Asset, FILE* _File)
{
	// 포인터가 nullptr 인지 아닌지 저장
	int bAsset = !!_Asset;
	fwrite(&bAsset, sizeof(int), 1, _File);

	if (bAsset)
	{
		// 텍스쳐 키, 경로 저장
		StringSave(_Asset->GetKey(), _File);
		StringSave(_Asset->GetPath(), _File);
	}
}


#include "Texture.h"
#include "Sound.h"
#include "Sprite.h"
#include "Flipbook.h"
Asset* LoadAsset(ASSET_TYPE _Type, FILE* _File)
{
	// 텍스쳐가 nullptr 인지 아닌지 불러온다
	int bAsset = 0;
	fread(&bAsset, sizeof(int), 1, _File);

	if (!bAsset)
	{		
		return nullptr;
	}

	// 텍스쳐 키, 경로 불러오기
	wstring Key, Path;

	StringLoad(Key, _File);
	StringLoad(Path, _File);

	switch (_Type)
	{
	case ASSET_TYPE::TEXTURE:
		return AssetMgr::GetInst()->LoadTexture(Key, Path);
	//case ASSET_TYPE::SOUND:
	//	  return AssetMgr::GetInst()->LoadSound(Key, Path);
	case ASSET_TYPE::SPRITE:
		return AssetMgr::GetInst()->LoadSprite(Key, Path);
	case ASSET_TYPE::FLIPBOOK:
		return AssetMgr::GetInst()->LoadFlipbook(Key, Path);
	}

	assert(nullptr);
	return nullptr;
}

void Saturate(float& _Ratio)
{
	if (_Ratio < 0.f)
		_Ratio = 0.f;
	if (1.f < _Ratio)
		_Ratio = 1.f;
}
