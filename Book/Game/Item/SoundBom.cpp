#include "stdafx.h"
#include "SoundBom.h"
#include "PlayerManagement.h"
#include "Game.h"
#include "Enemy.h"

namespace
{
	const float SOUNDRANGE = 120.0f;
	const float EFFECTSIZE = 1.0f;
}
SoundBom::SoundBom()
{

}
SoundBom::~SoundBom()
{
	m_soundEffect->Stop();
	DeleteGO(m_soundEffect);
}
bool SoundBom::Start()
{
	Item::Start();
	EffectEngine::GetInstance()->ResistEffect(0,u"Assets/effect/soundeffect/soundeffect.efk");
	m_soundEffect = NewGO<EffectEmitter>(0);
	m_soundEffect->Init(0);
	//�G�t�F�N�g�̑傫�����w�肷��
	m_soundEffect->SetScale(Vector3::One*EFFECTSIZE);
	//�G�t�F�N�g�̍��W�̐ݒ�
	m_soundEffect->SetPosition(m_position);
	//�f�B�A�N�e�B�u
	Deactivate();
	return true;
}
void SoundBom::Update()
{
	Item::Update();
	m_soundEffect->SetPosition(m_position);
}
void SoundBom::ItemHit()
{
	//�߂��ɂ���G�l�~�[������T��
	for (int i = 0; i<m_game->GetEnemyList().size(); i++)
	{
		Vector3 diff = m_playerManagement->GetPosition() - m_game->GetEnemyList()[i]->GetPosition();
		if (diff.Length() < pow(SOUNDRANGE, 2.0f))
		{
			m_game->GetEnemyList()[i]->SetHitSoundBullet(true);
		}
		
	}
	Deactivate();
}