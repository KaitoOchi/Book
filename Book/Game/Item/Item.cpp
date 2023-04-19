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
	m_sphereCollider.Create(1.0f);
	
	return true;
}
void Item::Update()
{
	ItemChange();
	ItemRange();
	ItemEffect();
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g�i�Ǘp�j
struct SweepResyltWall:public btCollisionWorld::ConvexResultCallback
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

void Item::ItemChange()
{
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		//�M���e�������Ă���
		m_enItemState = m_enItem_Flash;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		//�����e�������Ă���
		m_enItemState = m_enItem_SoundBom;
	}
}

void Item::ItemRange()
{
	//�߂��ɂ���G�l�~�[������T��
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
		//�n�_�̓v���C���[�̍��W
		start.setOrigin(btVector3(playerPosition.x, playerPosition.y, playerPosition.z));
		//�I�_�̓G�l�~�[�̍��W
		end.setOrigin(btVector3(enemyPosition.x,enemyPosition.y,enemyPosition.z));
		
		SweepResyltWall callback;
		//�R���C�_�[���n�_����I�_�܂ł𓮂����āB
		//�Փ˂��邩�ǂ������ׂ�
		PhysicsWorld::GetInstance()->ConvexSweepTest(
			(const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

		//�ǂƏՓ˂���
		if (callback.isHit == true)
		{
			//�v���C���[�͌������Ă��Ȃ�
			return;
		}
		//�ǂƏՓ˂��Ă��Ȃ�
		//�G�l�~�[�t���O��true�ɁB
		GetHitEnemyList()[i]->SetHitFlashBullet(true);
	
	}

	
	
}

void Item::ItemEffect()
{
}