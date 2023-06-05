#include "stdafx.h"
#include "Gage.h"

#include "Game.h"
#include "Player3D.h"
#include "Enemy.h"
#include "Wipe.h"
#include "Fade.h"
#include "GameManager.h"
#include "Enemy_Increase.h"
#include "GoalSprite.h"

namespace
{
	const int		GAUGE_MAX = 10;																			//ゲージの最大値
	const int		LEVEL_MAX = 3;																			//レベルの最大値
	const float		VIGILANCE_TIME_MAX = 2.0f;																//警戒度の増加のクールダウン
	const float		VIGILANCE_DOWN_TIME = 5.0f;																//警戒度の減少のクールダウン
	const float		LEVEL_TIME_MAX = 1.0f;																	//レベルアニメーションタイマーの最大値
	const Vector2	VIGIRANCE_SIZE = Vector2(66.0f, 76.0f);													//警戒度の値画像のサイズ
	const Vector2	BASE_SIZE = Vector2(553.0f, 154.0f);													//ベースの画像のサイズ
	const Vector2	VIGIRANCE_POSITION = Vector2(280.0f, 350.0f);											//ゲージ画像の座標
	const Vector3	BASE_POSITION = Vector3(230.0f + VIGIRANCE_POSITION.x, VIGIRANCE_POSITION.y, 0.0f);		//ベース画像の座標
	const Vector3	LEVELUP_POSITION = Vector3(240.0f + VIGIRANCE_POSITION.x, VIGIRANCE_POSITION.y, 0.0f);	//レベル画像の座標
}

Gage::Gage()
{

}

Gage::~Gage()
{
	DeleteGO(m_wipe);
}

bool Gage::Start()
{
	m_wipe = NewGO<Wipe>(0, "wipe");

	//ゲームオブジェクトを検索
	m_game = FindGO<Game>("game");
	m_fade = FindGO<Fade>("fade");
	m_enemy_Increase = FindGO<Enemy_Increase>("enemyIncrease");
	m_player3D = FindGO<Player3D>("player3d");

	//背景画像の設定
	m_baseRender.Init("Assets/sprite/UI/Gauge/base.DDS", BASE_SIZE.x, BASE_SIZE.y);
	m_baseRender.SetPosition(BASE_POSITION);
	m_baseRender.Update();

	//警戒度の値画像の設定
	for (int i = 0; i < GAUGE_MAX; i++) {
		m_vigilanceRender[i].Init("Assets/sprite/UI/Gauge/gaugeCount.DDS", VIGIRANCE_SIZE.x, VIGIRANCE_SIZE.y);
		m_vigilanceRender[i].SetPosition(Vector3((40.0f * i ) + VIGIRANCE_POSITION.x, VIGIRANCE_POSITION.y, 0.0f));
		m_Color += 0.008 * i;
		
		m_vigilanceRender[i].SetMulColor(Vector4(0.0f, m_Color, m_Color, 1.0f));
		m_vigilanceRender[i].Update();
	}

	//レベル画像の設定
	m_LeverUPRender[0].Init("Assets/sprite/UI/Gauge/1.DDS", BASE_SIZE.x, BASE_SIZE.y);
	m_LeverUPRender[1].Init("Assets/sprite/UI/Gauge/2.DDS", BASE_SIZE.x, BASE_SIZE.y);
	m_LeverUPRender[2].Init("Assets/sprite/UI/Gauge/3.DDS", BASE_SIZE.x, BASE_SIZE.y);
	m_LeverUPRender[3].Init("Assets/sprite/UI/Gauge/max_base.DDS", 154.0f, 154.0f);
	m_LeverUPRender[3].SetPosition(Vector3{ 435.0f + VIGIRANCE_POSITION.x ,VIGIRANCE_POSITION.y ,0.0 });
	m_LeverUPRender[3].Update();
	for (int i = 0; i < LEVEL_MAX; i++) {
		m_LeverUPRender[i].SetPosition(LEVELUP_POSITION);
		m_LeverUPRender[i].Update();
	}

	//maxフォントの設定
	m_maxLeverRender.Init("Assets/sprite/UI/Gauge/max.DDS", BASE_SIZE.x, BASE_SIZE.y);
	m_maxLeverRender.SetScale(Vector3(0.2f, 1.0f, 0.0f));
	m_maxLeverRender.SetPosition(Vector3{ 435.0f + VIGIRANCE_POSITION.x, VIGIRANCE_POSITION.y, 0.0f });
	m_maxLeverRender.Update();


	return true;
}

void Gage::Update()
{
	if (m_leverState == m_enLever_MAX) {

		Gage_MAX();
		Gauge_Move();
	}

	m_vigilanceTime -= g_gameTime->GetFrameDeltaTime();

	if (m_vigilanceGage != 0&& m_leverState != m_enLever_MAX) {
		GageDown();
	}

	if (m_GetState != m_leverState)
	{
		m_Color = 0.7f;
		Gage_ColorChange();
		m_GetState = m_leverState;
	}
	

}

void Gage::GageUp(const int GageUp, const bool isEnemy)
{
	//ゲージタイマーのクールダウンがまだなら増加しない
	if (m_vigilanceTime > 0.0f) {
		return;
	}

	//捕まっている最中なら増加しない
	if (m_player3D->m_playerState == m_player3D->m_enPlayer_Catching) {
		return;
	}

	//発見SEを鳴らす
	SoundSource* se = NewGO<SoundSource>(0);
	if (isEnemy) {
		se->Init(10);
	}
	else {
		se->Init(3);
	}
	se->Play(false);
	se->SetVolume(GameManager::GetInstance()->GetSFX());

	//発見されていないなら
	if (!m_isFind) {
		if (GameManager::GetInstance()->GetGameState() != GameManager::enState_GetTreasure) {
			//BGMを変更する
			GameManager::GetInstance()->SetBGM(22);
		}
		m_isFind = true;
	}

	//ゲージを増加する
	if (m_vigilanceGage < 11 && m_leverState != m_enLever_MAX) {
		m_vigilanceGage += GageUp;
	}
	
	//ゲージ増加タイマーを設定
	m_vigilanceTime = VIGILANCE_TIME_MAX;

	VigilaceLeverChange();
}

void Gage::GageDown()
{
	//ゲージタイマーのクールダウンがまだなら減少しない
	if (m_vigilanceTime > -VIGILANCE_DOWN_TIME) {
		return;
	}

	for (int i = 0; i < m_game->GetEnemyList().size(); i++)
	{
		//エネミーの中に追跡中のステートがいるなら、減少しない
		if (m_game->GetEnemyList()[i]->m_ActState== m_game->GetEnemyList()[i]->m_ActState_Tracking||
			m_game->GetEnemyList()[i]->m_ActState == m_game->GetEnemyList()[i]->m_ActState_Charge)
		{
			m_HitTime = 5.0f;
			return;
		}
	}

	m_HitTime-= g_gameTime->GetFrameDeltaTime();
	//エネミーから見つかってしばらく経過したら
	if (m_HitTime < 0.0f)
	{
		m_vigilaceDownTime -= g_gameTime->GetFrameDeltaTime();
		if (m_vigilaceDownTime < 0.0f)
		{
			//警戒度を減少させる
			if (m_vigilanceGage < 11)
			{
				m_vigilanceGage -= 1;

				if (m_isFind) {
					//BGM��ύX
					GameManager::GetInstance()->SetBGM(21);
					m_isFind = false;
				}
			}
			m_vigilaceDownTime = 2.0f;
		}
	}
}

void Gage::VigilaceLeverChange()
{
	//警戒度が最大を超えたら、レベルを上げる
	if (m_vigilanceGage >= 11 && m_leverState != m_enLever_MAX)
	{
		switch (m_leverState)
		{
		case Gage::m_enLever_1:
			m_leverState = m_enLever_2;
			m_vigilanceGage = 0;
			m_wipe->Reset();
			m_enemy_Increase->Enemy_Open();
			break;
		case Gage::m_enLever_2:
			m_leverState = m_enLever_3;
			m_vigilanceGage = 0;
			m_wipe->Reset();
			m_enemy_Increase->Enemy_Open();
			break;
		case Gage::m_enLever_3:
			m_vigilanceGage = 10;
			m_enemy_Increase->Enemy_Open();
			break;
		case Gage::m_enLever_MAX:
			
			break;
		}
	}
}

void Gage::Gage_ColorChange()
{
	switch (m_leverState)
	{
	case Gage::m_enLever_1:
		for (int i = 0; i < GAUGE_MAX; i++)
		{
			m_Color += 0.008 * i;

			m_vigilanceRender[i].SetMulColor(Vector4(0.0f, m_Color, m_Color, 1.0f));
			m_vigilanceRender[i].Update();
		}
		break;
	case Gage::m_enLever_2:
		for (int i = 0; i < GAUGE_MAX; i++)
		{
			m_Color += 0.008 * i;

			m_vigilanceRender[i].SetMulColor(Vector4(m_Color, m_Color,0.0f, 1.0f));
			m_vigilanceRender[i].Update();
		}
		break;
	case Gage::m_enLever_3:
		for (int i = 0; i < GAUGE_MAX; i++)
		{
			m_Color += 0.008 * i;

			m_vigilanceRender[i].SetMulColor(Vector4(m_Color, 0.0f, 0.0f, 1.0f));
			m_vigilanceRender[i].Update();
		}
		break;
	default:
		break;
	}
}

void Gage::Gage_MAX()
{
	if (!m_MaxEnd) {
		return;
	}

	//警戒度を最大にする
	m_vigilanceGage = 10;
	for (int i = 0; i < GAUGE_MAX; i++) {
		m_Color += 0.008 * i;

		m_vigilanceRender[i].SetMulColor(Vector4(m_Color, 0.0f, 0.0f, 1.0f));
		m_vigilanceRender[i].Update();
	}

	//フェードが終了しているなら
	if (m_fade->IsFade() == false) {
		//ワイプのリセット
		m_wipe->Reset();
		//目標画像を出す
		GoalSprite* goalSprite = NewGO<GoalSprite>(0, "goalSprite");
		goalSprite->SetSpriteNum(true);
		//増援のエネミーを出現
		for (int i = 0; i < 3; i++)
		{
			m_enemy_Increase->Enemy_Open();
		}
		m_MaxEnd = false;
	}
	
}

void Gage::Gauge_Move()
{
	m_gaugeTimer += g_gameTime->GetFrameDeltaTime() * 2.0f;

	if (m_gaugeTimer > LEVEL_TIME_MAX) {
		m_gaugeTimer = -0.5f;
	}

	//スケールを調整
	m_gaugeScale = ((pow(m_gaugeTimer, 2.0f) * -3.0f) + (2.0f * m_gaugeTimer)) * 0.2f;
	m_gaugeScale = min(max(m_gaugeScale, 0.0f), 0.1f);

	//MAX画像の設定
	m_maxLeverRender.SetScale(Vector3(0.3f + m_gaugeScale, 0.7f + m_gaugeScale, 0.0f));
	m_maxLeverRender.Update();
}


void Gage::Render(RenderContext& rc)
{
	//背景画像の描画
	m_baseRender.Draw(rc);

	//警戒度の画像を描画
	for (int i = 0; i < m_vigilanceGage; i++) {
		m_vigilanceRender[i].Draw(rc);
	}

	//レベル画像を描画
	m_LeverUPRender[m_leverState].Draw(rc);

	//レベル最大の画像を描画
	if (m_leverState == m_enLever_MAX) {
		m_maxLeverRender.Draw(rc);
	}
}