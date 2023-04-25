#include "stdafx.h"
#include "GameUI.h"
#include "PlayerManagement.h"
namespace
{
	const Vector3	GAGE_SPRITE_POSITION = { -700.0f, 300.0f, 0.0f };	//�Q�[�W�摜�̈ʒu
	const Vector3	TIME_FONT_POSITION = { -100.0f, 500.0f, 0.0f };		//�^�C���̈ʒu
	const Vector3   VIGILANCE_POSITION = { 500.0f,350.0f,0.0f };
	const float		YSIZE = 154.0f;										//�c�̑傫��
	const float     XSIZE = 553.0f;										//���̑傫��
	const float		GAGE_MAX = 300.0f;									//�Q�[�W�̍ő�l
	const float		TIME_MAX = 180.0f;									//�ő�c�莞��
	const float		VIGILANCE_TIME_MAX = 2.0f;							//�x���l�̍ő厞��
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
	//�Q�[�W�̘g�摜�̐ݒ�
	m_gageFrameSpriteRender.Init("Assets/sprite/UI/gageFrame_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans);
	m_gageFrameSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageFrameSpriteRender.Update();

	//�Q�[�W�摜�̐ݒ�
	m_gageSpriteRender.Init("Assets/sprite/UI/gage_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans, 1);
	m_gageSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageSpriteRender.Update();

	//�^�C���̐ݒ�
	m_timeFontRender.SetPosition(TIME_FONT_POSITION);
	m_timeFontRender.SetPivot(Vector2(0.5f, 0.5f));
	m_timeFontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_timeFontRender.SetShadowParam(true, 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//�x���x�Q�[�W�摜�̐ݒ�
	m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_1.DDS", XSIZE, YSIZE);
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
	//���Ԃ��v������
	m_timer -= g_gameTime->GetFrameDeltaTime();

	//�����v�Z
	int m = m_timer / 60;
	//�b���v�Z
	float s = (int)m_timer % 60;
	//�~���b���v�Z
	s += m_timer - (int)m_timer;

	wchar_t debugText[255];
	swprintf_s(debugText, 255, L"Time %d:%05.02f", m, s);
	m_timeFontRender.SetText(debugText);


	//�x���l�̃N�[���_�E����ݒ�
	m_vigilanceTime -= g_gameTime->GetFrameDeltaTime();
}
void GameUI::ChangeGage()
{
	if (m_playerManagement->m_enMnanagementState == m_playerManagement->m_enPlayer_2DChanging && m_gage > 0) {
		//���炷
		m_gage -= 1.0f;
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize.x = GAGE_MAX - m_gage;
	}
	else if (m_playerManagement->m_enMnanagementState == m_playerManagement->m_enPlayer_2DChanging)
	{
		m_playerManagement->PlayerChange3D();
	}
	if (m_playerManagement->m_enMnanagementState == m_playerManagement->m_enPlayer_3DChanging && m_gage < GAGE_MAX) {
		//���₷
		m_gage += 2.0f;
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize.x = GAGE_MAX - m_gage;
	}
}
void GameUI::Vigilance(int GageUp)
{
	//�N�[���_�E�����܂��Ȃ�
	if (m_vigilanceTime > 0.0f) {
		return;
	}

	m_vigilanceGage += GageUp;
	if (m_vigilanceGage != m_Gitgage)
	{
		m_Gitgage = m_vigilanceGage;
		VigilanceChange();
	}

	m_vigilanceTime = VIGILANCE_TIME_MAX;
}
void GameUI::VigilanceChange()
{
	switch (m_Gitgage)
	{
	case 1:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_1.DDS", XSIZE, YSIZE);
		break;
	case 2:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_2.DDS", XSIZE, YSIZE);
		break;
	case 3:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_3.DDS", XSIZE, YSIZE);
		break;
	case 4:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_4.DDS", XSIZE, YSIZE);
		break;
	case 5:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_5.DDS", XSIZE, YSIZE);
		break;
	case 6:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_6.DDS", XSIZE, YSIZE);
		break;
	case 7:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_7.DDS", XSIZE, YSIZE);
		break;
	case 8:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_8.DDS", XSIZE, YSIZE);
		break;
	case 9:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_9.DDS", XSIZE, YSIZE);
		break;
	case 10:
		m_vigilanceRender.Init("Assets/sprite/CautionTimeGauge/gage_10.DDS", XSIZE, YSIZE);
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