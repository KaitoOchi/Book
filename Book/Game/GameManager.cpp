#include "stdafx.h"
#include "GameManager.h"


GameManager* GameManager::m_instance = nullptr;


GameManager::GameManager()
{
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/sound_test.wav");
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
	//BGM‚Ìíœ’†‚È‚ç
	if (!m_isDeleteBGM) {
		return;
	}

	//BGM‚ÌÄ¶‚ğ~‚ß‚é
	if (m_timer < 0.0f) {
		m_isDeleteBGM = false;
		m_bgm->Stop();
	}

	m_timer -= g_gameTime->GetFrameDeltaTime();
	m_bgm->SetVolume(m_timer);
}