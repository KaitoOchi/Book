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
	const float		VIGILANCE_TIME_MAX = 2.0f;							//�x���l�̍ő厞��
	const float		VIGILANCE_DOWN_TIME = 5.0f;						//�x���x�����R�������鎞��
	const float		BASEYSIZE = 154.0f;									//base�c�̑傫��
	const float     BASEXSIZE = 553.0f;									//base���̑傫��
	const float		VIGIRANCE_XSIZE = 66.0f;							//�x���x�̏c�̑傫��
	const float		VIGIRANCE_YSIZE = 76.0f;							//�x���x�̉��̑傫��
	const int		MAXGAGECOUNT = 10;
	const int		MAXLEVERCOUNT = 3;
	const float		RENDER_YPOSITION = 350.0f;
	const float		RENDER_XPOSITION=280.0f;
	const Vector3	BASE_POSITION(230.0f+ RENDER_XPOSITION, RENDER_YPOSITION, 0.0f);
	const Vector3	LEVERUPPOSITION(240.0f+ RENDER_XPOSITION, RENDER_YPOSITION, 0.0f);
	const float		GAUGE_LEVER_MAX = 0.3f;
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
	//�Q�[���̏�������Ă���
	m_game = FindGO<Game>("game");

	m_wipe = NewGO<Wipe>(0, "wipe");

	m_fade = FindGO<Fade>("fade");

	m_enemy_Increase = FindGO<Enemy_Increase>("enemyIncrease");

	m_player3D = FindGO<Player3D>("player3d");

	//��Ղ̍X�V
	m_baseRender.Init("Assets/sprite/UI/Gauge/base.DDS", BASEXSIZE, BASEYSIZE);
	m_baseRender.SetPosition(BASE_POSITION);
	m_baseRender.Update();
	//�Q�[�W�̍쐬
	for (int i = 0; i < MAXGAGECOUNT; i++)
	{
		m_vigilanceRender[i].Init("Assets/sprite/UI/Gauge/gaugeCount.DDS", VIGIRANCE_XSIZE, VIGIRANCE_YSIZE);
		m_vigilanceRender[i].SetPosition(Vector3(40.0f * i+RENDER_XPOSITION, RENDER_YPOSITION, 0.0f));
		m_Color += 0.008 * i;
		
		m_vigilanceRender[i].SetMulColor(Vector4(0.0f, m_Color, m_Color, 1.0f));
		m_vigilanceRender[i].Update();
	}

	//���x���̍쐬
	m_LeverUPRender[0].Init("Assets/sprite/UI/Gauge/1.DDS", BASEXSIZE, BASEYSIZE);
	m_LeverUPRender[1].Init("Assets/sprite/UI/Gauge/2.DDS", BASEXSIZE, BASEYSIZE);
	m_LeverUPRender[2].Init("Assets/sprite/UI/Gauge/3.DDS", BASEXSIZE, BASEYSIZE);
	m_LeverUPRender[3].Init("Assets/sprite/UI/Gauge/max_base.DDS", 154.0f, 154.0f);
	m_LeverUPRender[3].SetPosition(Vector3{ 435.0f + RENDER_XPOSITION,RENDER_YPOSITION ,0.0 });
	m_LeverUPRender[3].Update();
	for (int i = 0; i < MAXLEVERCOUNT ; i++)
	{
		m_LeverUPRender[i].SetPosition(LEVERUPPOSITION);
		m_LeverUPRender[i].Update();
	}

	//maxフォントの描画
	m_maxLeverRender.Init("Assets/sprite/UI/Gauge/max.DDS", BASEXSIZE, BASEYSIZE);
	m_maxLeverRender.SetScale(Vector3{ 0.2f,1.0f,0.0f });
	m_maxLeverRender.SetPosition(Vector3{ 435.0f + RENDER_XPOSITION, RENDER_YPOSITION, 0.0f });
	m_maxLeverRender.Update();


	return true;
}

void Gage::Update()
{

	if (m_leverState == m_enLever_MAX)
	{
		Gage_MAX();
		Gauge_Move();
	}
	//�x���x�̃N�[���^�C����v�Z
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
	//�N�[���_�E�����܂��Ȃ�
	if (m_vigilanceTime > 0.0f) {
		return;
	}

	//�߂܂��Ă���Ȃ�SE�𗬂��Ȃ�
	if (m_player3D->m_playerState == m_player3D->m_enPlayer_Catching) {
		return;
	}

	//��������o��
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
		if (GameManager::GetInstance()->GetGameState() != GameManager::enState_GetTreasure) {
			//BGM��ύX
			GameManager::GetInstance()->SetBGM(22);
		}
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
		//���C�v��o��
		m_wipe->Reset();
		//�ڕW�摜��o��
		GoalSprite* goalSprite = NewGO<GoalSprite>(0, "goalSprite");
		goalSprite->SetSpriteNum(true);
		for (int i = 0; i < 3; i++)
		{
			m_enemy_Increase->Enemy_Open();
		}
		
		m_MaxEnd = false;
	}
	
}

void Gage::Gauge_Move()
{
	if (!m_maxState)
	{
		m_gaugeScaleY += 1.0f * g_gameTime->GetFrameDeltaTime();
		m_gaugeScaleY = min(m_gaugeScaleY, GAUGE_LEVER_MAX);
		m_gaugeScaleX += 0.2 * g_gameTime->GetFrameDeltaTime();
		if (m_gaugeScaleY == GAUGE_LEVER_MAX)
		{
			m_maxState = true;
		}
	}
	else
	{
		m_gaugeScaleY -= 1.0f * g_gameTime->GetFrameDeltaTime();
		m_gaugeScaleY = max(m_gaugeScaleY, 0.0f);
		m_gaugeScaleX -= 0.2 * g_gameTime->GetFrameDeltaTime();
		if (m_gaugeScaleY == 0.0f)
		{
			m_maxState = false;
		}
	}
	m_maxLeverRender.SetScale(Vector3{ 0.2f+ m_gaugeScaleX,1.0f+ m_gaugeScaleY,0.0f });
	m_maxLeverRender.Update();
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
		m_maxLeverRender.Draw(rc);
		break;
	default:
		break;
	}
	
}