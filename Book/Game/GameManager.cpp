#include "stdafx.h"
#include "GameManager.h"


GameManager* GameManager::m_instance = nullptr;


GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::Update()
{
	//�Q�[�����Ȃ�
	if (m_gameState == enState_Game) {

		//�|�[�Y�{�^���̐ݒ�
		if (g_pad[0]->IsTrigger(enButtonStart) && m_posState == true)
		{
			GameObjectManager::GetInstance()->SetStop(true);
			m_posState = false;
		}
		else if (g_pad[0]->IsTrigger(enButtonStart) && m_posState == false)
		{
			GameObjectManager::GetInstance()->SetStop(false);
			m_posState = true;
		}
	}
}