#include "stdafx.h"
#include "Gage.h"

#include "Game.h"
#include "Enemy.h"
#include "Wipe.h"
#include "Fade.h"
#include "GameManager.h"
#include "Enemy_Increase.h"

namespace
{
	const float		VIGILANCE_TIME_MAX = 2.0f;							//警戒値の最大時間
	const float		VIGILANCE_DOWN_TIME = 5.0f;						//警戒度が自然減少する時間
	const float		BASEYSIZE = 154.0f;									//base縦の大きさ
	const float     BASEXSIZE = 553.0f;									//base横の大きさ
	const float		VIGIRANCE_XSIZE = 66.0f;							//警戒度の縦の大きさ
	const float		VIGIRANCE_YSIZE = 76.0f;							//警戒度の横の大きさ
	const int		MAXGAGECOUNT = 10;
	const int		MAXLEVERCOUNT = 4;
	const float		RENDER_YPOSITION = 350.0f;
	const float		RENDER_XPOSITION=280.0f;
	const Vector3	BASE_POSITION(230.0f+ RENDER_XPOSITION, RENDER_YPOSITION, 0.0f);
	const Vector3	LEVERUPPOSITION(240.0f+ RENDER_XPOSITION, RENDER_YPOSITION, 0.0f);
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
	//ゲームの情報を持ってくる
	m_game = FindGO<Game>("game");

	m_wipe = NewGO<Wipe>(0, "wipe");

	m_fade = FindGO<Fade>("fade");

	m_enemy_Increase = FindGO<Enemy_Increase>("enemyIncrease");

	//基盤の更新
	m_baseRender.Init("Assets/sprite/UI/Gauge/base.DDS", BASEXSIZE, BASEYSIZE);
	m_baseRender.SetPosition(BASE_POSITION);
	m_baseRender.Update();
	//ゲージの作成
	for (int i = 0; i < MAXGAGECOUNT; i++)
	{
		m_vigilanceRender[i].Init("Assets/sprite/UI/Gauge/gaugeCount.DDS", VIGIRANCE_XSIZE, VIGIRANCE_YSIZE);
		m_vigilanceRender[i].SetPosition(Vector3(40.0f * i+RENDER_XPOSITION, RENDER_YPOSITION, 0.0f));
		m_Color += 0.008 * i;
		
		m_vigilanceRender[i].SetMulColor(Vector4(0.0f, m_Color, m_Color, 1.0f));
		m_vigilanceRender[i].Update();
	}

	//レベルの作成
	m_LeverUPRender[0].Init("Assets/sprite/UI/Gauge/1.DDS", BASEXSIZE, BASEYSIZE);
	m_LeverUPRender[1].Init("Assets/sprite/UI/Gauge/2.DDS", BASEXSIZE, BASEYSIZE);
	m_LeverUPRender[2].Init("Assets/sprite/UI/Gauge/3.DDS", BASEXSIZE, BASEYSIZE);
	m_LeverUPRender[3].Init("Assets/sprite/UI/Gauge/max.DDS", BASEXSIZE, BASEYSIZE);
	for (int i = 0; i < MAXLEVERCOUNT ; i++)
	{
		m_LeverUPRender[i].SetPosition(LEVERUPPOSITION);
		m_LeverUPRender[i].Update();
	}

	return true;
}

void Gage::Update()
{

	if (m_leverState == m_enLever_MAX)
	{
		Gage_MAX();
	}
	//警戒度のクールタイムを計算
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
	//クールダウンがまだなら
	if (m_vigilanceTime > 0.0f) {
		return;
	}

	//発見音を出す
	SoundSource* se = NewGO<SoundSource>(0);

	if (isEnemy) {
		se->Init(10);
	}
	else {
		se->Init(3);
	}
	se->Play(false);
	se->SetVolume(GameManager::GetInstance()->GetSFX());

	if (!m_isFind) {
		//BGMを変更
		GameManager::GetInstance()->SetBGM(22);
		m_isFind = true;
	}

	if (m_vigilanceGage < 11 && m_leverState != m_enLever_MAX) {
		m_vigilanceGage += GageUp;
	}
	
	m_vigilanceTime = VIGILANCE_TIME_MAX;

	VigilaceLeverChange();
}

void Gage::GageDown()
{
	if (m_vigilanceTime > -VIGILANCE_DOWN_TIME) {
		return;
	}

	for (int i = 0; i < m_game->GetEnemyList().size(); i++)
	{
		if (m_game->GetEnemyList()[i]->m_ActState== m_game->GetEnemyList()[i]->TRACKING||
			m_game->GetEnemyList()[i]->m_ActState == m_game->GetEnemyList()[i]->CHARGE)
		{
			m_HitTime = 5.0f;
			return;
		}
	}
	m_HitTime-= g_gameTime->GetFrameDeltaTime();
	if (m_HitTime < 0.0f)
	{
		m_vigilaceDownTime -= g_gameTime->GetFrameDeltaTime();

		if (m_vigilaceDownTime < 0.0f)
		{
			if (m_vigilanceGage < 11)
			{
				m_vigilanceGage -= 1;

				if (m_isFind) {
					//BGMを変更
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
	if (m_vigilanceGage >= 11 && m_leverState != m_enLever_MAX)
	{
		switch (m_leverState)
		{
		case Gage::m_enLever_1:
			m_leverState = m_enLever_2;
			m_vigilanceGage = 0;
			m_wipe->Reset();
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
		for (int i = 0; i < MAXGAGECOUNT; i++)
		{
			m_Color += 0.008 * i;

			m_vigilanceRender[i].SetMulColor(Vector4(0.0f, m_Color, m_Color, 1.0f));
			m_vigilanceRender[i].Update();
		}
		break;
	case Gage::m_enLever_2:
		for (int i = 0; i < MAXGAGECOUNT; i++)
		{
			m_Color += 0.008 * i;

			m_vigilanceRender[i].SetMulColor(Vector4(m_Color, m_Color,0.0f, 1.0f));
			m_vigilanceRender[i].Update();
		}
		break;
	case Gage::m_enLever_3:
		for (int i = 0; i < MAXGAGECOUNT; i++)
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
	if (!m_MaxEnd)
	{
		return;
	}
	m_vigilanceGage = 10;
	for (int i = 0; i < MAXGAGECOUNT; i++)
	{
		m_Color += 0.008 * i;

		m_vigilanceRender[i].SetMulColor(Vector4(m_Color, 0.0f, 0.0f, 1.0f));
		m_vigilanceRender[i].Update();
	}
	if (m_fade->IsFade() == false)
	{
		m_wipe->Reset();
		m_enemy_Increase->Enemy_Open();
		m_MaxEnd = false;
	}
	
}


void Gage::Render(RenderContext& rc)
{
	m_baseRender.Draw(rc);

	for (int i = 0; i < m_vigilanceGage; i++) {
		m_vigilanceRender[i].Draw(rc);
	}

	switch (m_leverState)
	{
	case Gage::m_enLever_1:
		m_LeverUPRender[0].Draw(rc);
		break;
	case Gage::m_enLever_2:
		m_LeverUPRender[1].Draw(rc);
		break;
	case Gage::m_enLever_3:
		m_LeverUPRender[2].Draw(rc);
		break;
	case Gage::m_enLever_MAX:
		m_LeverUPRender[3].Draw(rc);
		break;
	default:
		break;
	}
	
}