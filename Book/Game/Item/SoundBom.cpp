#include "stdafx.h"
#include "SoundBom.h"
#include "PlayerManagement.h"
#include "Game.h"
#include "Enemy.h"
namespace
{
	const float SOUNDRANGE = 120.0f;
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
	Deactivate();
	return true;
}
void SoundBom::Update()
{
	Item::Update();
}
void SoundBom::ItemHit()
{
	//近くにいるエネミーたちを探す
	for (int i = 0; m_game->GetEnemyList().size(); i++)
	{
		Vector3 diff = m_playerManagement->GetPosition() - m_game->GetEnemyList()[i]->GetPosition();
		if (diff.Length() < pow(SOUNDRANGE, 2.0f))
		{
			m_game->GetEnemyList()[i]->SetHitSoundBullet(true);
		}
		
	}
	Deactivate();
}