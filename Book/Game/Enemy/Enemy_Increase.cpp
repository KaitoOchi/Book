#include "stdafx.h"
#include "Enemy_Increase.h"
#include "Enemy_Normal.h"
#include "Enemy_Charge.h"
#include "Enemy_Clear.h"
#include "Game.h"
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

}

void Enemy_Increase::NewEnemy(int number)
{
	switch (number)
	{
	/*case 0:
		Enemy_Normal * normal = NewGO<Enemy_Normal>(0, "enemyNormal");
		normal->SetPosition(m_position);
		normal->SetRotation(m_rotation);
		normal->SetScale(m_scale);
		m_game->GetEnemyList().emplace_back(normal);
		break;
	case 1:
		Enemy_Charge * charge = NewGO<Enemy_Charge>(0, "enemyCharge");
		charge->SetPosition(m_position);
		charge->SetRotation(m_rotation);
		charge->SetScale(m_scale);
		m_game->GetEnemyList().emplace_back(charge);
		break;
	case 2:
		Enemy_Clear * clear = NewGO<Enemy_Clear>(0,"enemyClear");
		clear->SetPosition(m_position);
		clear->SetRotation(m_rotation);
		clear->SetScale(m_scale);
		m_game->GetEnemyList().emplace_back(clear);
		break;
	default:
		break;*/
	}
}