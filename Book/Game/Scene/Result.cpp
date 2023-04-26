#include "stdafx.h"
#include "Result.h"

#include "Fade.h"
#include "Title.h"
#include "Game.h"
#include "GameUI.h"

namespace
{
	const float CAN_INPUT_GAMECLEAR = 5.0f;						//�Q�[���N���A���̓��͉\����
	const float CAN_INPUT_GAMEOVER = 2.0f;						//�Q�[���I�[�o�[���̓��͉\����
	const float	ENABLE_TIME[5] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };	//�\���\�ɂȂ鎞��
}

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
	//�Q�[���N���A�Ȃ�
	case enState_GameClear:
		InitGameClear();
		break;

	//�Q�[���I�[�o�[�Ȃ�
	case enState_GameOver:
		InitGameOver();
		break;
	}

	//�J�[�\���̐ݒ�
	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);

	//�t�F�[�h�̏���
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Result::InitGameClear()
{
	int lightNum = 0;

	GameUI* gameUI = FindGO<GameUI>("gameUI");

	//�w�i�摜�̐ݒ�
	m_backGroundSpriteRender.Init("Assets/sprite/UI/result/base.DDS", 1920.0f, 1080.0f);

	//���x���̃f�[�^���g�p���ă^�C�g���摜��ǂݍ���
	m_level2DRender.Init("Assets/level2D/result.casl", [&](Level2DObjectData& objData) {
		//���O����v���Ă�����B
		if (objData.EqualObjectName("missionComplete!") == true) {
			//�����摜��ݒ�
			m_stateSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_stateSpriteRender.SetPosition(objData.position);
			m_stateSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("result_rank") == true) {
			m_explainSpriteRender[0].Init(objData.ddsFilePath, objData.width, objData.height);
			m_explainSpriteRender[0].SetPosition(objData.position);

			m_rankFontRender.SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualObjectName("result_text") == true) {
			m_explainSpriteRender[1].Init(objData.ddsFilePath, objData.width, objData.height);
			m_explainSpriteRender[1].SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualObjectName("light") == true) {
			m_fontPosition[lightNum] = objData.position;
			lightNum++;
			return true;
		}
		return false;
	});

	for (int i = 0; i < 2; i++) {
		m_explainSpriteRender[i].SetScale(Vector3(0.9f, 0.9f, 0.0f));
		m_explainSpriteRender[i].Update();
	}

	//���U���g��ۑ�
	//m_score[0] = gameUI->GetTime();
	m_score[0] = 99.0f;
	m_score[1] = 1.0f;
	m_score[2] = GameManager::GetInstance()->GetSearchNum();

	//�����N�\���̐ݒ�
	m_rankFontRender.SetText(L"A");
	m_rankFontRender.SetScale(5.0f);
	m_rankFontRender.SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	m_rankFontRender.SetShadowParam(true, 1.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//�X�R�A�����̐ݒ�
	for (int i = 0; i < 4; i++) {
		//�X�R�A���e�L�X�g�ɕϊ�
		wchar_t text[255];
		swprintf_s(text, 255,
			L"%10d",
			m_score[i]
		);
		m_messageFontRender[i].SetText(text);
		m_messageFontRender[i].SetPosition(m_fontPosition[i]);
		m_messageFontRender[i].SetPivot(Vector2(1.0f, 0.5f));
	}

	//�ҋ@���Ԃ̐ݒ�
	m_canInputTime = CAN_INPUT_GAMECLEAR;
}

void Result::InitGameOver()
{
	//�w�i�摜�̐ݒ�
	m_backGroundSpriteRender.Init("Assets/sprite/UI/gameOver/base.DDS", 1920.0f, 1080.0f);

	//�^�C�g���摜�̐ݒ�
	m_stateSpriteRender.Init("Assets/sprite/UI/gameOver/gameOver.DDS", 516.0f, 138.0f);
	m_stateSpriteRender.SetPosition(Vector3(-550.0f, 400.0f, 0.0f));
	m_stateSpriteRender.Update();

	//�ڍ׉摜�̐ݒ�
	m_explainSpriteRender[0].Init("Assets/sprite/UI/gameOver/retry.DDS", 317.0f, 149.0f);
	m_explainSpriteRender[0].SetPosition(Vector3(-600.0f, 90.0f, 0.0f));
	m_explainSpriteRender[1].Init("Assets/sprite/UI/gameOver/giveup.DDS", 481.0f, 152.0f);
	m_explainSpriteRender[1].SetPosition(Vector3(-600.0f, -150.0f, 0.0f));

	for (int i = 0; i < 2; i++) {
		m_explainSpriteRender[i].SetPosition(Vector3(-600.0f, 90.0f + (-150.0f * i), 0.0f));
		m_explainSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_explainSpriteRender[i].Update();
	}

	//�ҋ@���Ԃ̐ݒ�
	m_canInputTime = CAN_INPUT_GAMEOVER;
}

void Result::Update()
{
	if (!IsEndAnimation()) {
		return;
	}

	//�t�F�[�h�̑ҋ@����
	if (m_isWaitFadeOut) {

		//�t�F�[�h�����I������
		if (!m_fade->IsFade()) {
			//�V�[���J��
			if (m_resultState == enState_GameClear) {
				NewGO<Title>(0, "title");
			}
			else if (m_cursor == 0) {
				NewGO<Game>(0, "game");
			}
			else {
				NewGO<Title>(0, "title");
			}
			DeleteGO(this);
		}

		return;
	}
	else {

		Input();
	}

	switch (m_resultState)
	{
		//�Q�[���N���A�Ȃ�
	case enState_GameClear:
		GameClear();
		break;

		//�Q�[���I�[�o�[�Ȃ�
	case enState_GameOver:
		GameOver();
		break;
	}
}

void Result::Input()
{
	//A�{�^������������
	if (g_pad[0]->IsTrigger(enButtonA)) {

		//�N���A�X�e�[�g�ŁA�\�����S�Ċ������Ă��Ȃ�
		if (m_resultState == enState_GameClear && m_timer < ENABLE_TIME[4]) {
			//�\�����X�L�b�v����
			m_timer = ENABLE_TIME[4];
		}
		else {
			//�t�F�[�h���n�߂�
			m_isWaitFadeOut = true;
			m_fade->StartFadeOut();
		}
	}

	//�\���{�^������������
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor++;
	}
	else if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor--;
	}
	m_cursor = min(max(m_cursor, 0), 1);
}

void Result::GameClear()
{
	//���Ԃ̏���
	m_timer += g_gameTime->GetFrameDeltaTime();
}

void Result::GameOver()
{
	//���Ԃ̏���
	m_timer += g_gameTime->GetFrameDeltaTime();
	if (m_timer > 1.0f)
		m_timer = -0.5f;

	// -t^2 + 2t
	m_alpha = fabsf(-pow(m_timer, 2.0f) + (2 * m_timer));
	m_alpha = min(m_alpha, 1.0f);
	m_alpha *= 3.0f;
	m_alpha = max(m_alpha, 1.0f);

	m_cursorSpriteRender.SetPosition(Vector3(-650.0f, 90.0f + (m_cursor * -240.0f), 0.0f));
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_cursorSpriteRender.Update();
}

void Result::Render(RenderContext& rc)
{
	m_backGroundSpriteRender.Draw(rc);
	m_stateSpriteRender.Draw(rc);

	if (!IsEndAnimation()) {
		return;
	}

	//�Q�[���N���A�Ȃ�
	if (m_resultState == enState_GameClear) {

		for (int i = 0; i < 4; i++) {
			if(m_timer > ENABLE_TIME[i])
				m_messageFontRender[i].Draw(rc);
		}

		if (m_timer > ENABLE_TIME[4]) {
			m_explainSpriteRender[0].Draw(rc);
			m_rankFontRender.Draw(rc);
		}
	}
	//�Q�[���I�[�o�[�Ȃ�
	else {
		m_cursorSpriteRender.Draw(rc);
		m_explainSpriteRender[0].Draw(rc);
	}
	m_explainSpriteRender[1].Draw(rc);
}
