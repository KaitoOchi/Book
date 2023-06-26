#include "stdafx.h"
#include "SoundBom.h"
#include "PlayerManagement.h"
#include "Game.h"
#include "Enemy.h"
#include "GameUI.h"
namespace
{
	const float SOUNDRANGE = 500.0f;
	const float EFFECTSIZE = 2.0f;
	const int	SOUNDNUMBER = 5;
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
	//アイテムの数を代入する
	m_soundCount = SOUNDNUMBER;
	m_gameUI->SetSoundSlot(m_soundCount);
	//ディアクティブ
	Deactivate();
	return true;
}
void SoundBom::Update()
{
	Item::Update();

	if (m_soundEffect != nullptr) {
		//m_soundEffect->SetTime(g_gameTime->GetFrameDeltaTime() * 5.0f);
	}
}

void SoundBom::ItemHit()
{
	//持ち物数を減らす
	m_soundCount -= 1;
	
	m_soundEffect = NewGO<EffectEmitter>(1);
	m_soundEffect->Init(1);
	//エフェクトの大きさを指定する
	m_soundEffect->SetScale(Vector3::One * EFFECTSIZE);
	//エフェクトの座標の設定
	m_soundEffect->SetPosition(Vector3(m_position.x,10.0f,m_position.z));
	m_soundEffect->SetTime(g_gameTime->GetFrameDeltaTime() * 5.0f);
	m_soundEffect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
	m_soundEffect->Play();
	
	//近くにいるエネミーたちを探す
	for (int i = 0; i<m_game->GetEnemyList().size(); i++)
	{
		//エネミーが行動しているなら
		if (!m_game->GetEnemyList()[i]->GetActiveFlag())
		{
			Vector3 diff = m_playerManagement->GetPosition() - m_game->GetEnemyList()[i]->GetPosition();
			if (diff.LengthSq() < pow(SOUNDRANGE, 2.0f))
			{
				//エネミーに当たった判定と座標を与える
				m_game->GetEnemyList()[i]->SetHearedSoundBulletFlag(true);
				m_game->GetEnemyList()[i]->SetItemPos(m_position);
			}
      
		}
		
	}

	m_gameUI->SetSoundSlot(m_soundCount);
	NotifySounding();
}

void SoundBom::Sounding()
{
	//エフェクトが出ていないなら
	if (m_soundEffect->IsPlay() == false)
	{
		//終わりを告げる
		NotifySoundEnd();
	}
}

void SoundBom::SoundEnd()
{
	//ディアクティブにする
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