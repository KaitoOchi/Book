#include "stdafx.h"
#include "Item.h"
#include "Game.h"
#include "PlayerManagement.h"
#include "Player3D.h"
#include "Enemy.h"
Item::Item()
{

}
Item::~Item()
{

}
bool Item::Start()
{
	m_game = FindGO<Game>("game");
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_player3D = FindGO<Player3D>("player3D");
	return true;
}
void Item::Update()
{
	ItemChange();

}

//衝突したときに呼ばれる関数オブジェクト（壁用）
struct SweepResyltWall:public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall)
		{
			//衝突したのは壁ではない
			return 0.0f;
		}
		//壁とぶつかったら
		//フラグをtrueに
		isHit = true;
		return 0.0f;

	}
};

void Item::ItemChange()
{
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		m_enItemState = m_enItem_Flash;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		m_enItemState = m_enItem_SoundBom;
	}
}

void Item::ItemRange()
{
	for (int i = 0; m_game->GetEnemyList().size(); i++)
	{
		Vector3 diff = m_playerManagement->GetPosition() - m_game->GetEnemyList()[i]->GetPosition();
		switch (m_enItemState)
		{
		case Item::m_enItem_Flash:
			if (diff.Length() < pow(60.0f,2.0f))
			{
				m_HitEnemyList.push_back(m_game->GetEnemyList()[i]);
			}
			break;
		case Item::m_enItem_SoundBom:
			if (diff.Length() < pow(120.0f, 2.0f))
			{
				m_HitEnemyList.push_back(m_game->GetEnemyList()[i]);
			}
			break;
		default:
			break;
		}
	}
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	for (int i = 0; GetHitEnemyList().size(); i++)
	{
		Vector3 enemyPosition= GetHitEnemyList()[i]->GetPosition();
		Vector3 playerPosition = m_playerManagement->GetPosition();
		//始点はエネミーの座標
		start.setOrigin(btVector3(enemyPosition.x,enemyPosition.y,enemyPosition.z));
		end.setOrigin(btVector3(playerPosition.x, playerPosition.y, playerPosition.z));
	}
	
	
}

void Item::ItemEffect()
{
}