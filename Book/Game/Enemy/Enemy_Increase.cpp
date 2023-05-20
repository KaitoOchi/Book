#include "stdafx.h"
#include "Enemy.h"
#include "Enemy_Increase.h"
#include "Enemy_Normal.h"
#include "Enemy_Charge.h"
#include "Enemy_Clear.h"
#include "PlayerManagement.h"
#include "Game.h"
#include "Gage.h"
Enemy_Increase::Enemy_Increase()
{

}
Enemy_Increase::~Enemy_Increase()
{

}

bool Enemy_Increase::Start()
{
	m_game = FindGO<Game>("game");
	m_gage = FindGO<Gage>("gage");
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	return	true;
}

void Enemy_Increase::Update()
{
	Enemy_Open();
}

void Enemy_Increase::Enemy_Open()
{
	if (m_increaseEnemy <= 2)
	{
		//エネミーの大きさを求める
		for (int i = 0; i < m_game->GetEnemyList().size(); i++)
		{
			//アクティブになっていないエネミーを探す
			if (m_game->GetEnemyList()[i]->IsActive()==false)
			{
				//プレイヤーとエネミーの距離を求める
				Vector3 pos = m_playerManagement->GetPosition() - m_game->GetEnemyList()[i]->GetPosition();
				//最も距離のある値を求める
				if (pos.LengthSq()> m_nearposition)
				{
					m_nearposition = pos.Length();
					m_ifPosition = m_game->GetEnemyList()[i]->GetPosition();
				}
			}
		}

		for (int i = 0; i < m_game->GetEnemyList().size(); i++)
		{
			//アクティブになっていないエネミーを探す
			if (!m_game->GetEnemyList()[i]->IsActive())
			{
				if (m_ifPosition.LengthSq() == m_game->GetEnemyList()[i]->GetPosition().LengthSq())
				{
					m_game->GetEnemyList()[i]->Activate();
				}
			}
		}
	}
}