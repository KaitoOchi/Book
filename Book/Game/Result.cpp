#include "stdafx.h"
#include "Result.h"

#include "Fade.h"
#include "Title.h"

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	switch (m_resultState)
	{
	case enState_GameClear:
		m_backGroundSpriteRender.Init("Assets/sprite/UI/result/base.DDS", 1920.0f, 1080.0f);
		break;

	case enState_GameOver:
		m_backGroundSpriteRender.Init("Assets/sprite/UI/gameOver/all.DDS", 1920.0f, 1080.0f);
		break;
	}

	m_cursorSpriteRender.Init("Assets/sprite/UI/gameOver/tryangle.DDS", 131.0f, 135.0f);

	//フェードの処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Result::Update()
{
	switch (m_resultState)
	{
		//ゲームクリアなら
	case enState_GameClear:
		GameClear();
		break;

		//ゲームオーバーなら
	case enState_GameOver:
		GameOver();
		break;
	}

	if (g_pad[0]->IsTrigger(enButtonA)) {
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void Result::GameClear()
{

}

void Result::GameOver()
{

}

void Result::Render(RenderContext& rc)
{
	m_backGroundSpriteRender.Draw(rc);
	m_cursorSpriteRender.Draw(rc);
}
