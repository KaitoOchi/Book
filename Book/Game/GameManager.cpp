#include "stdafx.h"
#include "GameManager.h"


GameManager* GameManager::m_instance = nullptr;


GameManager::GameManager()
{
	//�L�����Z�����̉�
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/title/cancel.wav");
	//���莞�̉�
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/title/enter.wav");
	//�I�����̉�
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/title/select.wav");

	//�^�C�g��BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(20, "Assets/sound/bgm/bgm_title.wav");

	//�Q�[�����C��BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(21, "Assets/sound/bgm/bgm_game_main.wav");

	//�Q�[������BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(22, "Assets/sound/bgm/bgm_find.wav");

	//���U���gBGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(23, "Assets/sound/bgm/bgm_result.wav");

	//�f�[�^�̃��[�h
	DataLoad();
}

GameManager::~GameManager()
{

}

void GameManager::Update()
{
	DeletingBGM();
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
		m_timer = 0.0f;
	}

	m_timer -= g_gameTime->GetFrameDeltaTime() * 10.0f;
	m_bgm->SetVolume(m_timer);
}