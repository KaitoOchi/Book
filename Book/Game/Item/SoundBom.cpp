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
	//エフェクトの大きさを指定する
	m_soundEffect->SetScale(Vector3::One*EFFECTSIZE);
	//エフェクトの座標の設定
	m_soundEffect->SetPosition(m_position);
	//ディアクティブ
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
	//近くにいるエネミーたちを探す
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