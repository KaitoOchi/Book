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

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g�i�Ǘp�j
struct SweepResyltWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ����ĂȂ�������
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall)
		{
			//�Փ˂����͕̂ǂł͂Ȃ�
			return 0.0f;
		}
		//�ǂƂԂ�������
		//�t���O��true��
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
	//�߂��ɂ���G�l�~�[������T��
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
			//�n�_�̓v���C���[�̍��W
			start.setOrigin(btVector3(playerPosition.x, playerPosition.y, playerPosition.z));
			//�I�_�̓G�l�~�[�̍��W
			end.setOrigin(btVector3(enemyPosition.x, enemyPosition.y, enemyPosition.z));
			SweepResyltWall callback;
			//�R���C�_�[���n�_����I�_�܂ł𓮂����āB
			//�Փ˂��邩�ǂ������ׂ�
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
            //�ǂƏՓ˂���
			if (callback.isHit == true)
			{
				//�v���C���[�͌������Ă��Ȃ�
				return;
			}
			//�ǂƏՓ˂��Ă��Ȃ�
			//�G�l�~�[�t���O��true�ɁB
			m_game->GetEnemyList()[i]->SetHitFlashBullet(true);
		}
	}
	Deactivate();
}