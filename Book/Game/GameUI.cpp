#include "stdafx.h"
#include "GameUI.h"


namespace
{
	const Vector3	GAGE_SPRITE_POSITION = { -700.0f, 300.0f, 0.0f };	//�Q�[�W�摜�̈ʒu
	const Vector3	TIME_FONT_POSITION = { -100.0f, 500.0f, 0.0f };		//�^�C���̈ʒu
	const float		GAGE_MAX = 245.0f;									//�Q�[�W�̍ő�l
	const float		TIME_MAX = 180.0f;									//�ő�c�莞��
}

GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	m_gage = GAGE_MAX;
	m_timer = TIME_MAX;

	//�Q�[�W�̘g�摜�̐ݒ�
	m_gageFrameSpriteRender.Init("Assets/sprite/UI/gageFrame_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans, false);
	m_gageFrameSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageFrameSpriteRender.Update();

	//�Q�[�W�摜�̐ݒ�
	m_gageSpriteRender.Init("Assets/sprite/UI/gage_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans, true);
	m_gageSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageSpriteRender.Update();

	//�^�C���̐ݒ�
	m_timeFontRender.SetPosition(TIME_FONT_POSITION);
	m_timeFontRender.SetPivot(Vector2(0.5f, 0.5f));
	m_timeFontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_timeFontRender.SetShadowParam(true, 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	return true;
}

void GameUI::Update()
{
	Time();

	//Debug
	// ��������������������Ă�
	//�Q�[�W�̗ʂ�ύX���鏈��
	if (g_pad[0]->IsTrigger(enButtonA)) {
		//���炷
		m_gage -= 5.0f;
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize = GAGE_MAX - m_gage;
	}
	if (g_pad[0]->IsTrigger(enButtonB)) {
		//���₷
		m_gage += 5.0f;
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize = GAGE_MAX - m_gage;
	}
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
}

void GameUI::Render(RenderContext& rc)
{
	m_gageFrameSpriteRender.Draw(rc);
	m_gageSpriteRender.Draw(rc);
	m_timeFontRender.Draw(rc);
}