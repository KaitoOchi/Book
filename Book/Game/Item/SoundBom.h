#pragma once
#include "Item.h"
class Game;
class SoundBom:public Item
{
public:
	SoundBom();
	~SoundBom()override;
	bool Start()override;
	void Update();
	void ItemHit()override;
	void Sounding();
	void SoundEnd();
	/// <summary>
	/// 音爆弾の開始の通知
	/// </summary>
	void NotifySoundStart()
	{
		m_soundState = m_enSoundState_Start;
	}

	/// <summary>
	/// 音爆弾の継続中の通知
	/// </summary>
	void NotifySounding()
	{
		m_soundState = m_enSoundState_Sounding;
	}

	/// <summary>
	/// 音爆弾の開始の通知
	/// </summary>
	void NotifySoundEnd()
	{
		m_soundState = m_enSoundState_End;
	}

	const int GetSoundBomNumber()
	{
		return m_soundCount;
	}

public:
	enum EnSoundState
	{
		m_enSoundState_Start,		//処理の開始
		m_enSoundState_Sounding,	//処理の継続中
		m_enSoundState_End,			//処理の終了
	};

	EnSoundState m_soundState = m_enSoundState_End;			//処理の終了
	
private:
	EffectEmitter* m_soundEffect=nullptr;
	int m_soundCount = 0;						//フラッシュボムの所持数
private:
	/// <summary>
	/// ステートの遷移
	/// </summary>
	void ManageState()override;
};

