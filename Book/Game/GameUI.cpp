#include "stdafx.h"
#include "GameUI.h"
#include "PlayerManagement.h"

namespace
{
	const Vector3	GAGE_SPRITE_POSITION = { -700.0f, 300.0f, 0.0f };	//ゲージ画像の位置
	const Vector3	TIME_FONT_POSITION = { -100.0f, 500.0f, 0.0f };		//タイムの位置
	const Vector3   VIGILANCE_POSITION = { 550.0f,100.0f,0.0f };
	const float		GAGE_MAX = 255.0f;									//ゲージの最大値
	const float		TIME_MAX = 180.0f;									//最大残り時間
}

GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_gage = GAGE_MAX;
	m_timer = TIME_MAX;
	//ゲージの枠画像の設定
	m_gageFrameSpriteRender.Init("Assets/sprite/UI/gageFrame_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans, false);
	m_gageFrameSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageFrameSpriteRender.Update();

	//ゲージ画像の設定
	m_gageSpriteRender.Init("Assets/sprite/UI/gage_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans, true);
	m_gageSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageSpriteRender.Update();

	//タイムの設定
	m_timeFontRender.SetPosition(TIME_FONT_POSITION);
	m_timeFontRender.SetPivot(Vector2(0.5f, 0.5f));
	m_timeFontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_timeFontRender.SetShadowParam(true, 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//警戒度ゲージ画像の設定
	m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_1.DDS",450,128);
	m_vigilanceRender.SetPosition(VIGILANCE_POSITION);
	m_vigilanceRender.Update();


	return true;
}

void GameUI::Update()
{
	Time();
	ChangeGage();
	
}

void GameUI::Time()
{
	//時間を計測する
	m_timer -= g_gameTime->GetFrameDeltaTime();

	//分を計算
	int m = m_timer / 60;
	//秒を計算
	float s = (int)m_timer % 60;
	//ミリ秒を計算
	s += m_timer - (int)m_timer;

	wchar_t debugText[255];
	swprintf_s(debugText, 255, L"Time %d:%05.02f", m, s);
	m_timeFontRender.SetText(debugText);
}
void GameUI::ChangeGage()
{
	if (m_playerManagement->m_enMnanagementState == m_playerManagement->m_enPlayer_2DChanging && m_gage > 0) {
		//減らす
		m_gage -= 1.0f;
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize = GAGE_MAX - m_gage;
	}
	else if (m_playerManagement->m_enMnanagementState == m_playerManagement->m_enPlayer_2DChanging)
	{
		m_playerManagement->PlayerChange3D();
	}
	if (m_playerManagement->m_enMnanagementState == m_playerManagement->m_enPlayer_3DChanging && m_gage < GAGE_MAX) {
		//増やす
		m_gage += 2.0f;
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize = GAGE_MAX - m_gage;
	}
}
void GameUI::Vigilance(int GageUp)
{
	m_vigilanceGage += GageUp;
	if (m_vigilanceGage != m_Gitgage)
	{
		m_Gitgage = m_vigilanceGage;
		VigilanceChange();
	}
}
void GameUI::VigilanceChange()
{
	switch (m_Gitgage)
	{
	case 1:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_1.DDS", 450, 128);
		break;
	case 2:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_2.DDS", 450, 128);
		break;
	case 3:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_3.DDS", 450, 128);
		break;
	case 4:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_4.DDS", 450, 128);
		break;
	case 5:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_5.DDS", 450, 128);
		break;
	case 6:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_6.DDS", 450, 128);
		break;
	case 7:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_7.DDS", 450, 128);
		break;
	case 8:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_8.DDS", 450, 128);
		break;
	case 9:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_9.DDS", 450, 128);
		break;
	case 10:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_10.DDS", 450, 128);
		break;
	default:
		break;
	}
	m_vigilanceRender.Update();
}
void GameUI::Render(RenderContext& rc)
{
	m_gageFrameSpriteRender.Draw(rc);
	m_gageSpriteRender.Draw(rc);
	//m_timeFontRender.Draw(rc);
	m_vigilanceRender.Draw(rc);
}