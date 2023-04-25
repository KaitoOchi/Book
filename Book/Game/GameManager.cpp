#include "stdafx.h"
#include "GameManager.h"


GameManager* GameManager::m_instance = nullptr;


GameManager::GameManager()
{
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/sound_test.wav");
}

GameManager::~GameManager()
{

}

void GameManager::Update()
{
	Pause();

	DeletingBGM();
}

void GameManager::Pause()
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

void GameManager::DeletingBGM()
{
	//BGM�̍폜���Ȃ�
	if (!m_isDeleteBGM) {
		return;
	}

	//BGM�̍Đ����~�߂�
	if (m_timer < 0.0f) {
		m_isDeleteBGM = false;
		m_bgm->Stop();
	}

	m_timer -= g_gameTime->GetFrameDeltaTime();
	m_bgm->SetVolume(m_timer);
}