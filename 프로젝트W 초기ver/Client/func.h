#pragma once

void DrawDebugRect(Vec2 _Center, Vec2 _Scale, PEN_TYPE _Pen, float _Life);

void DrawDebugCircle(Vec2 _Center, float _Radius, PEN_TYPE _Pen, float _Life);

void DrawDebugLine(Vec2 _Start, Vec2 _End, PEN_TYPE _Pen, float _Life);


void CreateActor(ACTOR_TYPE _Type, class Actor* _Actor);

void ChangeLevel(LEVEL_TYPE _Next);


bool IsValid(Actor*& _Actor);


Vec2 Rotate(Vec2 _Dir, float _Angle);


// 파일에 문자열 저장 / 불러오기
void StringSave(const wstring& _String, FILE* _File);
void StringLoad(wstring& _String, FILE* _File);

// 사용 에셋 정보 저장 / 불러오기
class Asset;
void SaveAsset(Asset* _Asset, FILE* _File);
Asset* LoadAsset(ASSET_TYPE _Type, FILE* _File);


void Saturate(float& _Ratio);


template<typename T, int Size>
void Delete_Arr(T(&Arr)[Size])
{
	for (int i = 0; i < Size; ++i)
	{
		if (nullptr != Arr[i])
		{
			delete Arr[i];
			Arr[i] = nullptr;
		}		
	}
}


template<typename T>
void Delete_Vec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		delete _vec[i];
	}
	_vec.clear();
}


template<typename T1, typename T2>
void Delete_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		delete iter->second;
	}

	_map.clear();
}

