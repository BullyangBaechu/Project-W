#pragma once

class Texture;
class Sprite;
class Flipbook;
class Sound;

class AssetMgr
{
	SINGLE(AssetMgr);
private:
	map<wstring, Texture*>	m_mapTex;
	map<wstring, Sprite*>	m_mapSprite;
	map<wstring, Flipbook*> m_mapFlipbook;
	map<wstring, Sound*>	m_mapSound;

	IDirectSound8*			m_SoundDevice;	// 사운드 카드 제어	
	Sound*					m_BGM;

public:
	void Init();
	void RegisterToBGM(Sound* _BGM);
	
private:
	void InitSound();
	void CreateFlipbook(Texture* _pAtlas, const wstring& _Name, int _SpriteCount, Vec2 _LeftTop, Vec2 _Slice);

public:
	IDirectSound8* GetSoundDevice() { return m_SoundDevice; }

	Texture* FindTexture(const wstring& _Key);
	Texture* LoadTexture(const wstring& _Key, const wstring& _RelativePath);
	Texture* CreateTexture(const wstring& _Key, UINT _Width, UINT _Height);

	Sprite* FindSprite(const wstring& _Key);
	Sprite* LoadSprite(const wstring& _Key, const wstring& _RelativePath);

	Flipbook* FindFlipbook(const wstring& _Key);
	Flipbook* LoadFlipbook(const wstring& _Key, const wstring& _RelativePath);

	Sound* FindSound(const wstring& _Key);
	Sound* LoadSound(const wstring& _Key, const wstring& _RelativePath);
};

