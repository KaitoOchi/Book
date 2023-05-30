#include "stdafx.h"
#include "Enemy.h"
#include "Enemy_Increase.h"
#include "Enemy_Normal.h"
#include "Enemy_Charge.h"
#include "Enemy_Clear.h"
#include "PlayerManagement.h"
#include "Game.h"
#include "Gage.h"
namespace
{
}
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
}

void Enemy_Increase::Enemy_Open()
{
	for (int i = 0; i < 2; i++)
	{
		//エネミーの大きさを求める
		for (int i = 0; i < m_game->GetEnemyList().size(); i++)
		{
			//アクティブになっていないエネミーを探す
			if (m_game->GetEnemyList()[i]->GetActiveFlag())
			{
				//プレイヤーとエネミーの距離を求める
				Vector3 pos = m_playerManagement->GetPosition() - m_game->GetEnemyList()[i]->GetPosition();
				//最も距離のある値を求める
				if (pos.LengthSq() > m_nearposition)
				{
					m_nearposition = pos.LengthSq();
					m_ifPosition = m_game->GetEnemyList()[i]->GetPosition();
				}
			}
		}

		for (int i = 0; i < m_game->GetEnemyList().size(); i++)
		{
			//アクティブになっていないエネミーを探す
			if (m_game->GetEnemyList()[i]->GetActiveFlag())
			{
				//座標が同じなら探しているエネミーなので
				if (m_ifPosition.LengthSq() == m_game->GetEnemyList()[i]->GetPosition().LengthSq())
				{
					//エネミーアクティブにする
					m_game->GetEnemyList()[i]->SetActiveFlag(false);
					Vector3 move{ 0.0f, 1000.0f, 0.0f };
					Vector3 pos=m_game->GetEnemyList()[i]->GetCharCon().Execute(move, 1.0f);
					m_game->GetEnemyList()[i]->GetModelRender().SetPosition(pos);
					m_nearposition = FLT_MIN;
					m_game->GetEnemyList()[i]->Update();
				}
			}
		}
	}
}