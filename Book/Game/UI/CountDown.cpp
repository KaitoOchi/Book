#include "stdafx.h"
#include "CountDown.h"

#include "PlayerManagement.h"


CountDown::CountDown()
{

}

CountDown::~CountDown()
{

}

bool CountDown::Start()
{
	//カウントダウン画像の設定
	m_countDownSpriteRender[0].Init("Assets/sprite/UI/countDown/start.dds", 977.0f, 254.0f);
	m_countDownSpriteRender[1].Init("Assets/sprite/UI/countDown/1.dds", 73.0f, 234.0f);
	m_countDownSpriteRender[2].Init("Assets/sprite/UI/countDown/2.dds", 161.0f, 234.0f);
	m_countDownSpriteRender[3].Init("Assets/sprite/UI/countDown/3.dds", 166.0f, 255.0f);

	return true;
}

void CountDown::Update()
{
	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 1.0f) {
		//カウントを進めてタイムをリセットする
		m_count--;
		m_timer = 0.0f;

		if (m_count < 0) {
			PlayerManagement* player = FindGO<PlayerManagement>("playerManagement");
			player->SetGameState(true);
			DeleteGO(this);
			return;
		}
	}

	//カウント画像の設定
	m_countDownSpriteRender[m_count].SetScale(Vector3(m_timer, m_timer, 0.0f));
	m_countDownSpriteRender[m_count].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer));
	m_countDownSpriteRender[m_count].Update();
}

void CountDown::Render(RenderContext& rc)
{
	m_countDownSpriteRender[m_count].Draw(rc);
}