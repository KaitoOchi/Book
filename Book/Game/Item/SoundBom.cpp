#include "stdafx.h"
#include "SoundBom.h"
#include "PlayerManagement.h"
#include "Game.h"
#include "Enemy.h"
#include "GameUI.h"
namespace
{
	const float SOUNDRANGE = 30.0f;
	const float EFFECTSIZE = 2.0f;
}
SoundBom::SoundBom()
{
	
}
SoundBom::~SoundBom()
{
	
}
bool SoundBom::Start()
{
	Item::Start();
	m_gameUI->SetSoundSlot(m_soundCount);
	//�f�B�A�N�e�B�u
	Deactivate();
	return true;
}
void SoundBom::Update()
{
	Item::Update();
}

void SoundBom::ItemHit()
{
	m_soundCount -= 1;
	
	m_soundEffect = NewGO<EffectEmitter>(1);
	m_soundEffect->Init(1);
	//�G�t�F�N�g�̑傫�����w�肷��
	m_soundEffect->SetScale(Vector3::One * EFFECTSIZE);
	//�G�t�F�N�g�̍��W�̐ݒ�
	m_soundEffect->SetPosition(Vector3(m_position.x,10.0f,m_position.z));
	m_soundEffect->Play();
	
	//�߂��ɂ���G�l�~�[������T��
	for (int i = 0; i<m_game->GetEnemyList().size(); i++)
	{
		Vector3 diff = m_playerManagement->GetPosition() - m_game->GetEnemyList()[i]->GetPosition();
		if (diff.Length() < pow(SOUNDRANGE, 2.0f))
		{
			m_game->GetEnemyList()[i]->SetHearedSoundBullet(true);
			m_game->GetEnemyList()[i]->SetItemPos(m_position);
		}
		
	}

	m_gameUI->SetSoundSlot(m_soundCount);
	NotifySounding();
}

void SoundBom::Sounding()
{
	if (m_soundEffect->IsPlay() == false)
	{
		NotifySoundEnd();
	}
}

void SoundBom::SoundEnd()
{
	Deactivate();
}

void SoundBom::ManageState()
{
	switch (m_soundState)
	{
	case SoundBom::m_enSoundState_Start:
		ItemHit();
		break;
	case SoundBom::m_enSoundState_Sounding:
		Sounding();
		break;
	case SoundBom::m_enSoundState_End:
		SoundEnd();
		break;
	default:
		break;
	}
}