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
	//�������̉�
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/game/find.wav");
	//�h�������[���̉�
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/result/dramroll.wav");
	//���ۂ̉�
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/result/cymbal.wav");
	//����
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/game/step2.wav");
	//���n��
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/game/jump_end.wav");
	//������
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/game/throw.wav");
	//�M�����̉�
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/game/flashbom.wav");
	//�G�����������Ƃ��̉�
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/game/findPlayer.wav");
	//���C�g���Ƃ炳�ꂽ��
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/event/light.wav");
	//�G�̑���
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/event/foot.wav");
	//�ϐg���̉�
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/game/change.wav");
	//����̉�
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/event/tresure.wav");
	//�J�E���g�_�E�����̉�
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/countDown/count.wav");
	//�J�E���g�_�E���I���̉�
	g_soundEngine->ResistWaveFileBank(16, "Assets/sound/countDown/count_end.wav");
	//Search������̓G���ĂԂƂ��̉�
	g_soundEngine->ResistWaveFileBank(17, "Assets/sound/game/whistle_sound.wav");

	//�^�C�g��BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(20, "Assets/sound/bgm/bgm_title.wav");
	//�Q�[�����C��BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(21, "Assets/sound/bgm/bgm_game_main.wav");
	//����BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(22, "Assets/sound/bgm/bgm_find.wav");
	//���������BGM��ݒ�
	g_soundEngine->ResistWaveFileBank(23, "Assets/sound/bgm/bgm_game_main2.wav");
	//�Q�[���I�[�o�[���U���gBGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(24, "Assets/sound/bgm/bgm_result.wav");
	//�Q�[���I�[�o�[���U���gBGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(25, "Assets/sound/bgm/bgm_result_clear.wav");
	//�I�[�v�j���OBGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(26, "Assets/sound/bgm/bgm_opening.wav");

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

	m_timer -= (g_gameTime->GetFrameDeltaTime() * 0.4f);
	m_bgm->SetVolume(m_timer);

	//BGM�̍Đ����~�߂�
	if (m_timer <= 0.0f) {
		m_isDeleteBGM = false;
		m_bgm->Stop();
		m_timer = 0.0f;
	}
}