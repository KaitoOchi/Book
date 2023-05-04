#include "stdafx.h"
#include "GameManager.h"


GameManager* GameManager::m_instance = nullptr;


GameManager::GameManager()
{
	//キャンセル時の音
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/title/cancel.wav");
	//決定時の音
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/title/enter.wav");
	//選択時の音
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/title/select.wav");

	//タイトルBGMの設定
	g_soundEngine->ResistWaveFileBank(20, "Assets/sound/sound_test.wav");

	//データのロード
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