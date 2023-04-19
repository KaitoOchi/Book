#include "stdafx.h"
#include "FlashBom.h"
#include "Enemy.h"
#include "Game.h"
#include "PlayerManagement.h"
FlashBom::FlashBom()
{

}
FlashBom::~FlashBom()
{

}
bool FlashBom::Start()
{
	Item::Start();
	m_sphereCollider.Create(1.0f);
	Deactivate();
	return true;
}

//衝突したときに呼ばれる関数オブジェクト（壁用）
struct SweepResyltWall :public btCollisionWorld::ConvexResultCallback
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

void FlashBom::Update()
{
	Item::Update();

}
void FlashBom::ItemHit()
{
	//近くにいるエネミーたちを探す
	for (int i = 0; m_game->GetEnemyList().size(); i++)
	{
		Vector3 diff = m_playerManagement->GetPosition() - m_game->GetEnemyList()[i]->GetPosition();
        if (diff.Length() < pow(60.0f, 2.0f))
		{
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			Vector3 enemyPosition = m_game->GetEnemyList()[i]->GetPosition();
			Vector3 playerPosition = m_playerManagement->GetPosition();
			//始点はプレイヤーの座標
			start.setOrigin(btVector3(playerPosition.x, playerPosition.y, playerPosition.z));
			//終点はエネミーの座標
			end.setOrigin(btVector3(enemyPosition.x, enemyPosition.y, enemyPosition.z));
			SweepResyltWall callback;
			//コライダーを始点から終点までを動かして。
			//衝突するかどうか調べる
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
            //壁と衝突した
			if (callback.isHit == true)
			{
				//プレイヤーは見つかっていない
				return;
			}
			//壁と衝突していない
			//エネミーフラグをtrueに。
			m_game->GetEnemyList()[i]->SetHitFlashBullet(true);
		}
	}
	Deactivate();
}