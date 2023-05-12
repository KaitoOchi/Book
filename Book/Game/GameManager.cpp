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
	//発見時の音
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/game/find.wav");
	//ドラムロールの音
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/result/dramroll.wav");
	//太鼓の音
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/result/cymbal.wav");
	//足音
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/game/step2.wav");
	//着地音
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/game/jump_end.wav");
	//投擲音
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/game/throw.wav");
	//閃光時の音
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/game/flashbom.wav");
	//敵が発見したときの音
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/game/findPlayer.wav");
	//ライトが照らされた音
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/event/light.wav");
	//敵の足音
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/event/foot.wav");
	//変身時の音
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/game/change.wav");

	//タイトルBGMの設定
	g_soundEngine->ResistWaveFileBank(20, "Assets/sound/bgm/bgm_title.wav");
	//ゲームメインBGMの設定
	g_soundEngine->ResistWaveFileBank(21, "Assets/sound/bgm/bgm_game_main.wav");
	//発見BGMの設定
	g_soundEngine->ResistWaveFileBank(22, "Assets/sound/bgm/bgm_find.wav");
	//お宝入手後のBGMを設定
	g_soundEngine->ResistWaveFileBank(23, "Assets/sound/bgm/bgm_game_main2.wav");
	//ゲームオーバーリザルトBGMの設定
	g_soundEngine->ResistWaveFileBank(24, "Assets/sound/bgm/bgm_result.wav");
	//ゲームオーバーリザルトBGMの設定
	g_soundEngine->ResistWaveFileBank(25, "Assets/sound/bgm/bgm_result_clear.wav");

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
		m_timer = 0.0f;
	}

	m_timer -= (g_gameTime->GetFrameDeltaTime() * 0.5f);
	m_bgm->SetVolume(m_timer);
}