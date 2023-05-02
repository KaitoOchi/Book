#include "stdafx.h"
#include "CountDown.h"


CountDown::CountDown()
{

}

CountDown::~CountDown()
{

}

bool CountDown::Start()
{
	m_countDownSpriteRender[0].Init("Assets/sprite/UI/countDown/1.dds", 73.0f, 234.0f);
	m_countDownSpriteRender[1].Init("Assets/sprite/UI/countDown/2.dds", 161.0f, 234.0f);
	m_countDownSpriteRender[2].Init("Assets/sprite/UI/countDown/3.dds", 166.0f, 255.0f);

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
			DeleteGO(this);
		}
	}
}

void CountDown::Render(RenderContext& rc)
{
	m_countDownSpriteRender[m_count].Draw(rc);
}