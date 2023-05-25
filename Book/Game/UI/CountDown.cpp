#include "stdafx.h"
#include "CountDown.h"

#include "Game.h"
#include "PlayerManagement.h"
#include "GameManager.h"
#include "Pause.h"
#include "GoalSprite.h"


CountDown::CountDown()
{

}

CountDown::~CountDown()
{
	NewGO<Pause>(2, "pause");
}

bool CountDown::Start()
{
	//カウントダウン画像の設定
	m_countDownSpriteRender[0].Init("Assets/sprite/UI/countDown/start.dds", 977.0f, 254.0f);
	m_countDownSpriteRender[1].Init("Assets/sprite/UI/countDown/1.dds", 73.0f, 234.0f);
	m_countDownSpriteRender[2].Init("Assets/sprite/UI/countDown/2.dds", 161.0f, 234.0f);
	m_countDownSpriteRender[3].Init("Assets/sprite/UI/countDown/3.dds", 166.0f, 255.0f);

	for (int i = 0; i < 4; i++) {
		m_countDownSpriteRender[i].SetPosition(Vector3(0.0f, 150.0f, 0.0f));
	}


	return true;
}

void CountDown::Update()
{
	m_timer -= g_gameTime->GetFrameDeltaTime();

	if (m_timer < 0.0f) {
		//カウントを進めてタイムをリセットする
		m_count--;
		m_timer = 1.0f;

		if (m_count < 0) {
			PlayerManagement* player = FindGO<PlayerManagement>("playerManagement");
			player->SetGameState(true);

			//ゲームクラスにゲーム中を通知
			Game* game = FindGO<Game>("game");
			game->NotifyDuringGamePlay();

			//目標画像を出す
			GoalSprite* goalSprite = NewGO<GoalSprite>(0, "goalSprite");
			goalSprite->InitSprite(false);

			DeleteGO(this);
			return;
		}
		else if (m_count == 0) {
			//カウント終了の音を出す
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(16);
			se->Play(false);
			se->SetVolume(GameManager::GetInstance()->GetSFX());
		}
		else {
			//カウントの音を出す
			SoundSource* se2 = NewGO<SoundSource>(0);
			se2->Init(15);
			se2->Play(false);
			se2->SetVolume(GameManager::GetInstance()->GetSFX());
		}
	}

	if (m_timer > 0.5f) {
		//カウント画像の設定
		m_countDownSpriteRender[m_count].SetScale(Vector3(m_timer, m_timer, 0.0f));
		m_countDownSpriteRender[m_count].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 2.0f - (m_timer * 2.0f)));
		m_countDownSpriteRender[m_count].Update();
	}
	
}

void CountDown::Render(RenderContext& rc)
{
	m_countDownSpriteRender[m_count].Draw(rc);
}