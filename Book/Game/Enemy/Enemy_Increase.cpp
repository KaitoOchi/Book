#include "stdafx.h"
#include "Enemy.h"
#include "Enemy_Increase.h"
#include "Enemy_Normal.h"
#include "Enemy_Charge.h"
#include "Enemy_Clear.h"
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
	return	true;
}

void Enemy_Increase::Update()
{
	Enemy_Open();
}

void Enemy_Increase::NewEnemy(int number)
{
	/*switch (number)
	{
	case 0:
		Enemy_Normal * normal = NewGO<Enemy_Normal>(0, "enemyNormal");
		normal->m_enemyType = Enemy::TYPE_NORMAL;
		normal->SetPosition(m_position);
		normal->SetRotation(m_rotation);
		normal->SetScale(m_scale);
		normal->Pass(0);
		m_game->GetEnemyList().emplace_back(normal);
		break;
	case 1:
		Enemy_Charge * charge = NewGO<Enemy_Charge>(0, "enemyCharge");
		charge->m_enemyType = Enemy::TYPE_CHARGE;
		charge->SetPosition(m_position);
		charge->SetRotation(m_rotation);
		charge->SetScale(m_scale);
		charge->Pass(1);
		m_game->GetEnemyList().emplace_back(charge);
		break;
	case 2:
		Enemy_Clear * clear = NewGO<Enemy_Clear>(0,"enemyClear");
		clear->m_enemyType = Enemy::TYPE_CLEAR;
		clear->SetPosition(m_position);
		clear->SetRotation(m_rotation);
		clear->SetScale(m_scale);
		clear-> Pass(2);
		m_game->GetEnemyList().emplace_back(clear);
		break;
	default:
		break;
	}*/
}

void Enemy_Increase::Enemy_Open()
{

}