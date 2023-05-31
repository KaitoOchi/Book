#include "stdafx.h"
#include "GameUI.h"
#include "PlayerManagement.h"
#include "Player3D.h"
#include "Game.h"
#include "Treasure.h"
namespace
{
	const Vector3	GAGE_SPRITE_POSITION = { -900.0f, 300.0f, 0.0f };	//ゲージ画像の位置
	const Vector3	TIME_FONT_POSITION = { -100.0f, 500.0f, 0.0f };		//タイムの位置
	const float		GAGE_MAX = 300.0f;									//ゲージの最大値
	const float		TIME_MAX = 600.0f;									//最大残り時間
	const float		MAXTIMEYPOSITION = 600.0f;							//タイムの一番大きい座標
	const float		SETTIMEYPOSITION = 500.0f;							//タイムの移動先Y座標
	const float		SETTIMEXPOSITION = -100.0f;							//タイムの移動先X座標
	
	const Vector3	ITEM_BASE_POSITION = { 580.0f,250.0f,0.0f };		//アイテムの背景画像の座標
	const Vector3	ITEM_FLASH_POSITION = { 550.0f,250.0f,0.0f };		//閃光弾の画像の座標
	const Vector3	FLASH_FONT_POSITION = { 670.0f,280.0f,0.0f };		//閃光弾の数のフォントの座標
	const Vector3	ITEM_SOUND_POSITION = { 615.0f,250.0f,0.0f };		//音爆弾の処理
	const Vector3	SOUND_FONT_POSITION = { 745.0f,280.0f,0.0f };		//閃光弾の数のフォントの座標
	
	const float		FLASH_SCALE_MAX = 1.3f;								//閃光弾UIの最大のスケール
	const float		SOUND_SCALE_MAX = 1.3f;								//音爆弾UIの最大のスケール
	
	const float		STAMINA_POSITION = 60.0f;						//スタミナベース画像の座標
	const float		STAMINA_COOL_TIME = 1.0f;							//スタミナが回復するまでの時間

	const double	PI = 3.14159;										//円周率
	const float		CIRCLE_SIZE_MAX = 0.0f;							//円形ゲージ最大
	const float		CIRCLE_SIZE_MIN = 360.0f;						//円形ゲージ最低
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
	m_game = FindGO<Game>("game");
	m_player3D = FindGO<Player3D>("player3d");
	m_treasure = FindGO<Treasure>("treaSure");
	m_gage = GAGE_MAX;
	m_timer = TIME_MAX;

	//ゲージの枠画像の設定
	m_gageFrameSpriteRender.Init("Assets/sprite/UI/gageFrame_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans);
	m_gageFrameSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageFrameSpriteRender.Update();

	//ゲージ画像の設定
	m_gageSpriteRender.Init("Assets/sprite/UI/gage_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans, 1);
	m_gageSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageSpriteRender.Update();

	//タイムの設定
	m_timeFontRender.SetPosition(TIME_FONT_POSITION);
	m_timeFontRender.SetPivot(Vector2(0.5f, 0.5f));
	m_timeFontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_timeFontRender.SetShadowParam(true, 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//アイテムの背景の設定
	m_itemBaseRender.Init("Assets/sprite/UI/ItemSlot/base2.DDS",136,79);
	m_itemBaseRender.SetPosition(ITEM_BASE_POSITION);
	m_itemBaseRender.Update();

	//閃光弾画像の設定
	m_itemFlashRender.Init("Assets/sprite/UI/ItemSlot/flashbom.DDS", 69, 69);
	m_itemFlashRender.SetPosition(ITEM_FLASH_POSITION);
	m_itemFlashRender.Update();

	//音爆弾画像の設定
	m_itemSoundRender.Init("Assets/sprite/UI/ItemSlot/soundbom.DDS", 69, 69);
	m_itemSoundRender.SetPosition(ITEM_SOUND_POSITION);
	m_itemSoundRender.Update();


	//スタミナゲージのベース画像の設定
	m_staminaBaseRender.Init("Assets/sprite/UI/stamina/stamina_base.DDS", 162.0f, 183.0f);
	m_staminaBaseRender.SetScale(Vector3(0.5f, 0.5f, 0.0f));
	m_staminaBaseRender.Update();

	//スタミナゲージ画像の設定
	m_staminaGageRender.Init("Assets/sprite/UI/stamina/stamina.DDS", 162.0f, 183.0f,AlphaBlendMode_Trans, 5);
	m_staminaGageRender.SetScale(Vector3(0.5f, 0.5f, 0.0f));
	m_staminaGageRender.Update();
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_staminaDegree * PI) / 180.0f;

	//円形ゲージのベース画像
	m_circleBaseSpriteRender.Init("Assets/sprite/UI/PressAndHoldGauge/base.DDS", 162.0f, 183.0f);
	m_circleBaseSpriteRender.SetScale(Vector3(0.75f, 0.75f, 0.0f));
	m_circleBaseSpriteRender.Update();

	//円形ゲージのゲージ画像
	m_circleSpriteRender.Init("Assets/sprite/UI/PressAndHoldGauge/gauge.DDS",157.0f, 178.0f, AlphaBlendMode_Trans, 5);
	m_circleSpriteRender.SetScale(Vector3(0.75f, 0.75f, 0.0f));
	m_circleSpriteRender.Update();
	//RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_degree * PI) / 180.0f;



	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.x = GAGE_MAX - m_gage;

	m_timePosition = MAXTIMEYPOSITION;

	return true;
}

void GameUI::Update()
{
	ItemSlot();

	ItemScaleUp();

	if (m_game->m_gameState == Game::m_enGameState_GameStart ||
		m_game->m_gameState == Game::m_enGameState_GameOver) {
		return;
	}

	Time();
	
	ChangeGage();
	


	if (m_treasure->GetHitState())
	{
		CircleChange();
	}
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

	if (m < 1)
	{
		m_timeFontRender.SetColor(Vector4{ 1.0f, 0.0f, 0.0f, 1.0f });
	}

	//タイムの表示
	TimeMove(m, s);

	wchar_t debugText[255];
	swprintf_s(debugText, 255, L"Time %d:%05.02f", m, s);
	m_timeFontRender.SetText(debugText);


	//警戒値のクールダウンを設定
	m_vigilanceTime -= g_gameTime->GetFrameDeltaTime();
}

void GameUI::TimeMove(const int& m, const int& s)
{
	if (m_isTimerEnable) {
		//タイマーを非表示
		if (s == 50 && m > 1) {
			m_isTimerEnable = false;
		}

		//タイマーの表示
		m_timePosition -= 100 * g_gameTime->GetFrameDeltaTime();
		m_timePosition = max(m_timePosition, SETTIMEYPOSITION);
	}
	else {
		//一分ごとにタイマーを表示
		if (s == 59) {
			m_isTimerEnable = true;
		}

		//タイマーの表示
		m_timePosition += 100 * g_gameTime->GetFrameDeltaTime();
		m_timePosition = min(m_timePosition, MAXTIMEYPOSITION);
	}

	m_timeFontRender.SetPosition(Vector3{ SETTIMEXPOSITION, m_timePosition, 0.0f });
}

void GameUI::ChangeGage()
{
	//ゲージが満タンなら
	if (m_gage != GAGE_MAX) {
		m_gaugeTimer += g_gameTime->GetFrameDeltaTime();
		m_gaugeTimer = min(m_gaugeTimer, 1.0f);
	}
	else {
		m_gaugeTimer -= g_gameTime->GetFrameDeltaTime();
		m_gaugeTimer = max(m_gaugeTimer, 0.0f);
	}

	//ゲージ画像を移動
	m_gageFrameSpriteRender.SetPosition(GAGE_SPRITE_POSITION + Vector3(m_gaugeTimer * 200.0f, 0.0f, 0.0f));
	m_gageFrameSpriteRender.Update();
	m_gageSpriteRender.SetPosition(GAGE_SPRITE_POSITION + Vector3(m_gaugeTimer * 200.0f, 0.0f, 0.0f));
	m_gageSpriteRender.Update();


	if (m_playerManagement->m_enMananagementState == m_playerManagement->m_enPlayer_2DChanging && m_gage > 0) {
		//減らす
		m_gage -= 1.0f;
		m_gage = max(m_gage, 0.0f);
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize.x = GAGE_MAX - m_gage;
	}
	else if (m_playerManagement->m_enMananagementState == m_playerManagement->m_enPlayer_2DChanging)
	{
		if (m_playerManagement->GetEnPlayerState() != Player::m_enPlayer_Caught &&
			m_playerManagement->GetEnPlayerState() != Player::m_enPlayer_Catching)
		{
			if (m_changeGaugeState)
			{

				m_playerManagement->SetChange(m_playerManagement->m_enPlayer_3DChanging);
			}
		}
	}
	if (m_playerManagement->m_enMananagementState == m_playerManagement->m_enPlayer_3DChanging && m_gage < GAGE_MAX) {
		//増やす
		m_gage += 2.0f;
		m_gage = min(m_gage, GAGE_MAX);
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize.x = GAGE_MAX - m_gage;
	}
}

void GameUI::ItemSlot()
{
	wchar_t flashText[255];
	swprintf_s(flashText,L"x%d", m_flashNumber);
	m_itemFlashNumber.SetText(flashText);
	m_itemFlashNumber.SetScale(0.5f);
	m_itemFlashNumber.SetPosition(FLASH_FONT_POSITION);

	wchar_t soundText[255];
	swprintf_s(soundText, L"x%d", m_soundNumber);
	m_itemSoundNumber.SetText(soundText);
	m_itemSoundNumber.SetPosition(SOUND_FONT_POSITION);
	m_itemSoundNumber.SetScale(0.5f);
}

void GameUI::ItemScaleUp()
{
	
	switch (m_player3D->GetItemState())
	{
	case Player::m_enItem_Flash:
		if (m_falshState)
		{
			//拡大する
			m_flashScale += 0.2f*g_gameTime->GetFrameDeltaTime();
			m_flashScale = min(m_flashScale, FLASH_SCALE_MAX);
			if (m_flashScale == FLASH_SCALE_MAX)
			{
				m_falshState = false;
			}
		}
		else
		{
			//縮小する
			m_flashScale -= 0.2f * g_gameTime->GetFrameDeltaTime();
			m_flashScale = max(m_flashScale, 1.0f);
			if (m_flashScale == 1.0f)
			{
				m_falshState = true;
			}
		}
		//閃光弾UIを設定する
		m_itemFlashRender.SetScale(Vector3(m_flashScale, m_flashScale, 0.0f));
		m_itemFlashRender.SetMulColor(Vector4 {10.0f,10.0f,10.0f,1.0f });
		m_itemFlashRender.Update();

		//音爆弾UIを初期化する
		m_soundScale = 1.0f;
		m_itemSoundRender.SetScale(Vector3(m_soundScale, m_soundScale, 0.0f));
		m_itemSoundRender.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
		m_itemSoundRender.Update();
		break;
	case Player::m_enItem_SoundBom:
		if (m_soundState)
		{
			//拡大する
			m_soundScale += 0.2f * g_gameTime->GetFrameDeltaTime();
			m_soundScale = min(m_soundScale, SOUND_SCALE_MAX);
			if (m_soundScale == SOUND_SCALE_MAX)
			{
				m_soundState = false;
			}
		}
		else
		{
			//縮小する
			m_soundScale -= 0.2f * g_gameTime->GetFrameDeltaTime();
			m_soundScale = max(m_soundScale, 1.0f);
			if (m_soundScale == 1.0f)
			{
				m_soundState = true;
			}
		}
		//音爆弾UIを設定する
		m_itemSoundRender.SetScale(Vector3(m_soundScale, m_soundScale, 0.0f));
		m_itemSoundRender.SetMulColor(Vector4{ 10.0f,10.0f,10.0f,1.0f });
		m_itemSoundRender.Update();

		//閃光弾UIを初期化する
		m_flashScale = 1.0f;
		m_itemFlashRender.SetScale(Vector3(m_flashScale, m_flashScale, 0.0f));
		m_itemFlashRender.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
		m_itemFlashRender.Update();
		break;
	default:
		break;
	}
}

void GameUI::StaminaGage(float stamina,Vector3 pos)
{
	float m_stamina = stamina;
	m_staminaDegree = 360.0f-(36*m_stamina);
	m_staminaDegree = max(m_staminaDegree, CIRCLE_SIZE_MAX);
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_staminaDegree * PI) / 180.0f;


	Vector3 position = pos;
	//ワールド座標からスクリーン座標を計算
	g_camera3D->CalcScreenPositionFromWorldPosition(m_spritePosition, position);
	m_staminaBaseRender.SetPosition(Vector3(m_spritePosition.x + 70.0f, m_spritePosition.y + STAMINA_POSITION, 0.0f));
	m_staminaGageRender.SetPosition(Vector3(m_spritePosition.x + 70.0f, m_spritePosition.y + STAMINA_POSITION, 0.0f));


	m_staminaBaseRender.Update();
	m_staminaGageRender.Update();
}

void GameUI::CircleChange()
{
	//ゲージの変更
	if (m_circleState)
	{
		m_degree -= 120.0f*g_gameTime->GetFrameDeltaTime();
		m_degree = max(m_degree, CIRCLE_SIZE_MAX);
	}
	else if(m_degree!= CIRCLE_SIZE_MAX)
	{
		m_degree += 36.0f * g_gameTime->GetFrameDeltaTime();
		m_degree = min(m_degree, 360.0f);
	}

	if (m_degree -CIRCLE_SIZE_MAX<=0.000001f)
	{
		m_circleMaxState = true;
	}
	
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_degree * PI) / 180.0f;

	//ゲージの座標の変更
	Vector3 m_woldPosition = m_treasure->GetPosition();
	g_camera3D->CalcScreenPositionFromWorldPosition(m_circleposition, m_woldPosition);


	m_circleBaseSpriteRender.SetPosition(Vector3{ m_circleposition.x,m_circleposition.y + 100.0f,0.0f });
	m_circleSpriteRender.SetPosition(Vector3{ m_circleposition.x,m_circleposition.y + 100.0f,0.0f });
	m_circleBaseSpriteRender.Update();
	m_circleSpriteRender.Update();
}


void GameUI::Render(RenderContext& rc)
{
	m_gageFrameSpriteRender.Draw(rc);
	m_gageSpriteRender.Draw(rc);

	m_timeFontRender.Draw(rc);
	
	m_itemBaseRender.Draw(rc);
	m_itemFlashRender.Draw(rc);
	m_itemSoundRender.Draw(rc);
	m_itemFlashNumber.Draw(rc);
	m_itemSoundNumber.Draw(rc);
	
	
	//円形ゲージの描画
	if (m_circleDrawState
		&&m_treasure->GetHitState()
		&& m_treasure->m_drewState == true) {
		m_circleSpriteRender.Draw(rc);
		m_circleBaseSpriteRender.Draw(rc);
	}



	if (m_playerManagement->GetStamina() != 10.0f&&
		!m_circleDrawState&&
		m_staminaDrawState)
	{
		m_staminaGageRender.Draw(rc);
		m_staminaBaseRender.Draw(rc);

	}

}