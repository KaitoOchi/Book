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

void Enemy_Increase::Enemy_Open()
{
	for (int i = 0; i < 2; i++)
	{
		//�G�l�~�[�̑傫�������߂�
		for (int i = 0; i < m_game->GetEnemyList().size(); i++)
		{
			//�A�N�e�B�u�ɂȂ��Ă��Ȃ��G�l�~�[��T��
			if (m_game->GetEnemyList()[i]->GetActiveFlag())
			{
				//�v���C���[�ƃG�l�~�[�̋��������߂�
				Vector3 pos = m_playerManagement->GetPosition() - m_game->GetEnemyList()[i]->GetPosition();
				//�ł������̂���l�����߂�
				if (pos.LengthSq() > m_nearposition)
				{
					m_nearposition = pos.LengthSq();
					m_ifPosition = m_game->GetEnemyList()[i]->GetPosition();
				}
			}
		}

		for (int i = 0; i < m_game->GetEnemyList().size(); i++)
		{
			//�A�N�e�B�u�ɂȂ��Ă��Ȃ��G�l�~�[��T��
			if (m_game->GetEnemyList()[i]->GetActiveFlag())
			{
				//���W�������Ȃ�T���Ă���G�l�~�[�Ȃ̂�
				if ( abs ( m_ifPosition.LengthSq() - m_game->GetEnemyList()[i]->GetPosition().LengthSq()) <= 0.001f)
				{
					//�G�l�~�[�A�N�e�B�u�ɂ���
					m_game->GetEnemyList()[i]->SetActiveFlag(false);

					Vector3 pos = m_game->GetEnemyList()[i]->GetPosition();
					pos.y = 0.0f;

					m_game->GetEnemyList()[i]->SetPosition(pos);

					m_game->GetEnemyList()[i]->GetCharCon().SetPosition(pos);

					m_game->GetEnemyList()[i]->GetModelRender().SetPosition(pos);
					m_game->GetEnemyList()[i]->GetModelRender().Update();

					m_nearposition = FLT_MIN;
					m_game->GetEnemyList()[i]->Update();
				}
			}
		}
	}
}