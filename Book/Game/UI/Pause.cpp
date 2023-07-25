#include "stdafx.h"
#include "Pause.h"

#include "GameManager.h"
#include "Game.h"
#include "Title.h"

namespace
{
	const Vector3	CURSOR_POS_MENU[3] = { Vector3(-250.0f, 125.0f, 0.0f),
										Vector3(-250.0f, -110.0f, 0.0f),
										Vector3(-250.0f, -345.0f, 0.0f) };	//���j���[��ʂ̃J�[�\�����W
	const int		CURSOR_MIN = 0;											//�J�[�\���̍ŏ��l�B
	const int		CURSOR_MAX = 2;											//�J�[�\���̍ő�l�B
	const float		CURSOR_MOVESPEED = -235.0f;								//�J�[�\�����x�B
	const float		CURSOR_TIMER_SPEED = 4.0f;								//�J�[�\���^�C�}�[�̑��x�B
	const float		CURSOR_TIMER_MAX = 1.0f;								//�J�[�\���^�C�}�[�̍ő�l�B
	const float		FADEOUT_TIME = 0.25f;									//�t�F�[�h�A�E�g�̎��ԁB
	const float		FADE_SPEED = 4.0f;										//�t�F�[�h���x�B
}

Pause::Pause()
{

}

Pause::~Pause()
{

}

bool Pause::Start()
{
	m_game = FindGO<Game>("game");

	//�w�i�摜�̐ݒ�
	m_backGroundSpriteRender.Init("Assets/sprite/black.DDS", 1920.0f, 1080.0f);
	m_backGroundSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
	m_backGroundSpriteRender.Update();

	//�|�[�Y���j���[�摜�̐ݒ�
	m_pauseSpriteRender.Init("Assets/sprite/UI/pause/pause_text.DDS", 517.0f, 845.0f);

	//�J�[�\���摜�̐ݒ�
	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);
	m_cursorSpriteRender.SetPosition(Vector3(-250.0f, 125.0f + (m_cursor * CURSOR_MOVESPEED), 0.0f));
	m_cursorSpriteRender.Update();

	return true;
}

void Pause::PauseUpdate()
{
	switch (m_pauseState)
	{
	//�|�[�Y���Ȃ�
	case enState_Pause:
		PauseScreen();
		break;
	//�Q�[�����Ȃ�
	case enState_Game:
		//�|�[�Y�{�^���������ꂽ��
		if (g_pad[0]->IsTrigger(enButtonStart)) {
			m_isPause = true;
			m_pauseState = enState_FadeOut;
			GameObjectManager::GetInstance()->SetStop(true);
			m_game->StopWallEffect();
		}
		break;
	//�t�F�[�h�C�����Ȃ�
	case enState_FadeIn:
		FadeIn();
		break;
	//�t�F�[�h�A�E�g���Ȃ�
	case enState_FadeOut:
		FadeOut();
		break;
	}
}

void Pause::PauseScreen()
{
	//�J�[�\���̈ړ����́A���͂��󂯕t���Ȃ�
	if (m_cursorTimer < CURSOR_TIMER_MAX) {
		CursorMove();
		return;
	}

	//�X�^�[�g�{�^������������
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		GameObjectManager::GetInstance()->SetStop(false);
		m_pauseState = enState_FadeIn;

		//�L�����Z�������o��
		PlaySE(0);
	}

	//A�{�^������������
	if (g_pad[0]->IsTrigger(enButtonA)) {

		GameObjectManager::GetInstance()->SetStop(false);

		switch (m_cursor)
		{
		case 1:
			m_game->GameDelete(1);
			m_game->NotifyGameBack();
			break;
		case 2:
			m_game->GameDelete(2);
			m_game->NotifyGameBack();
			break;
		}

		//���艹���o��
		PlaySE(1);

		m_pauseState = enState_FadeIn;
	}

	//��{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor--;

		int cursor = m_cursor;
		m_cursor = min(max(m_cursor, CURSOR_MIN), CURSOR_MAX);

		if (m_cursor == cursor) {
			//�I�������o��
			PlaySE(2);
			m_cursorTimer = 0.0f;
		}
		m_nextCursor = 1;
	}
	//���{�^���������ꂽ��
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor++;

		int cursor = m_cursor;
		m_cursor = min(max(m_cursor, CURSOR_MIN), CURSOR_MAX);

		if (m_cursor == cursor) {
			//�I�������o��
			PlaySE(2);
			m_cursorTimer = 0.0f;
		}
		m_nextCursor = -1;
	}
}

void Pause::CursorMove()
{
	m_cursorTimer += g_gameTime->GetFrameDeltaTime() * CURSOR_TIMER_SPEED;

	// -t^2 + 2t
	float rate = ((pow(m_cursorTimer, 2.0f) * -1.0f) + (2.0f * m_cursorTimer));
	rate = min(rate, CURSOR_TIMER_MAX);

	//�J�[�\�����ړ�
	m_cursorPos.Lerp(rate, CURSOR_POS_MENU[m_cursor + m_nextCursor], CURSOR_POS_MENU[m_cursor]);
	m_cursorSpriteRender.SetPosition(m_cursorPos);
	m_cursorSpriteRender.Update();
}

void Pause::FadeIn()
{
	//�^�C�}�[������������
	m_timer -= g_gameTime->GetFrameDeltaTime();

	SetSprite();

	if (m_timer < 0.0f) {
		//�J�[�\���̏�����
		m_cursor = 0;
		m_cursorSpriteRender.SetPosition(Vector3(-250.0f, 125.0f + (m_cursor * CURSOR_MOVESPEED), 0.0f));
		m_cursorSpriteRender.Update();

		m_pauseState = enState_Game;
		m_isPause = false;

		//���ԃG�t�F�N�g�̍ĊJ
		m_game->PlayWallEffect();
	}
}

void Pause::FadeOut()
{
	//�^�C�}�[�𑝉�������
	m_timer += g_gameTime->GetFrameDeltaTime();

	SetSprite();

	if (m_timer > FADEOUT_TIME) {
		m_pauseState = enState_Pause;
	}
}

void Pause::SetSprite()
{
	//�����x��ݒ�
	m_backGroundSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer * (FADE_SPEED / 2.0f)));
	m_backGroundSpriteRender.Update();
	m_pauseSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer * FADE_SPEED));
	m_pauseSpriteRender.Update();
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer * FADE_SPEED));
	m_cursorSpriteRender.Update();
}

void Pause::Render(RenderContext& rc)
{
	PauseUpdate();

	//�|�[�Y��ʂ̕`��
	if (m_isPause) {
		m_backGroundSpriteRender.Draw(rc);
		m_pauseSpriteRender.Draw(rc);
		m_cursorSpriteRender.Draw(rc);
	}
}