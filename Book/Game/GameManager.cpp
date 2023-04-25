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
	//ゲーム中なら
	if (m_gameState == enState_Game) {

		//ポーズボタンの設定
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
	//BGMの削除中なら
	if (!m_isDeleteBGM) {
		return;
	}

	//BGMの再生を止める
	if (m_timer < 0.0f) {
		m_isDeleteBGM = false;
		m_bgm->Stop();
	}

	m_timer -= g_gameTime->GetFrameDeltaTime();
	m_bgm->SetVolume(m_timer);
}