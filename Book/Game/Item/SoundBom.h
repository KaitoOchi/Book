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
	/// �����e�̊J�n�̒ʒm
	/// </summary>
	void NotifySoundStart()
	{
		m_soundState = m_enSoundState_Start;
	}

	/// <summary>
	/// �����e�̌p�����̒ʒm
	/// </summary>
	void NotifySounding()
	{
		m_soundState = m_enSoundState_Sounding;
	}

	/// <summary>
	/// �����e�̊J�n�̒ʒm
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
		m_enSoundState_Start,		//�����̊J�n
		m_enSoundState_Sounding,	//�����̌p����
		m_enSoundState_End,			//�����̏I��
	};

	EnSoundState m_soundState = m_enSoundState_End;			//�����̏I��
	
private:
	EffectEmitter* m_soundEffect=nullptr;
	int m_soundCount = 0;						//�t���b�V���{���̏�����
private:
	/// <summary>
	/// �X�e�[�g�̑J��
	/// </summary>
	void ManageState()override;
};

