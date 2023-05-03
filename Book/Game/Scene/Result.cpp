#include "stdafx.h"
#include "Result.h"

#include "Fade.h"
#include "Title.h"
#include "Game.h"

namespace
{
	const char		RANK[4] = { 'A', 'B', 'C', 'D' };						//�����N�ꗗ
	const wchar_t* GET_TREASURE_TEXT[2] = {L"    FAILED", L" SUCCUESS!" };	//����擾�������ǂ����̃e�L�X�g
	const int		SCORE_RANK[4] = { 100000, 80000, 60000, 50000 };				//�����N�̏���
	const float		CAN_INPUT = 6.0f;										//���͉\����
	const float		ENABLE_TIME[5] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };		//�\���\�ɂȂ鎞��
	const Vector3	GAMEOVER_SPRITE_POS = { -150.0f, 0.0f, 0.0f };			//�Q�[���I�[�o�[�摜�̍��W
}

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	InitSprite();

	InitScore();

	//�ҋ@���Ԃ̐ݒ�
	m_canInputTime = CAN_INPUT;

	//�t�F�[�h�̏���
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Result::InitScore()
{
	//���U���g��ۑ�
	m_score[1] = m_resultState;
	m_score[2] = GameManager::GetInstance()->GetSearchNum();
	//m_score[2] = 5;
	m_score[3] = (m_score[0] * 100) +
		(m_score[1] * 50000) +
		(m_score[2] * -500);

	char rank = 'E';
	//�X�R�A���烉���N��ݒ�
	for (int i = 0; i < 4; i++) {
		//�X�R�A�̂ق�����Ȃ�
		if (m_score[3] > SCORE_RANK[i]) {
			rank = RANK[i];
			break;
		}
	}

	//�t�@�C���p�X�Ƀ����N�����킹��
	wchar_t path[255];
	swprintf_s(path, 255,
		L"Assets/sprite/UI/result/rank/%c.DDS",
		rank
	);
	char finalFilePath[100];
	wcstombs(finalFilePath, path, sizeof(path));

	//�����N�\���̐ݒ�
	m_rankSpriteRender.Init(finalFilePath, 306.0f, 312.0f);
	m_rankSpriteRender.SetPosition(Vector3(250.0f, -150.0f, 0.0f));
	m_rankSpriteRender.Update();

	int m = m_score[0] / 60;
	int s = m_score[0] % 60;

	//�X�R�A�����̐ݒ�
	for (int i = 0; i < 4; i++) {
		//�X�R�A���e�L�X�g�ɕϊ�
		wchar_t text[255];
		switch (i) {
		case 0:
			swprintf_s(text, 255,
				L"      %d:%02d",
				m,
				s
			);
			break;

		case 1:
			swprintf_s(text, 255,
				L"%s",
				GET_TREASURE_TEXT[m_resultState]
			);
			break;

		default:
			swprintf_s(text, 255,
				L"%10d",
				m_score[i]
			);
			break;
		}
		//�X�R�A������ݒ�
		m_messageFontRender[i].SetText(text);
		m_messageFontRender[i].SetPosition(m_fontPosition[i]);
		m_messageFontRender[i].SetPivot(Vector2(1.0f, 0.5f));
		m_messageFontRender[i].SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_messageFontRender[i].SetShadowParam(true, 1.5, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//���z�̐ݒ�
	wchar_t text[255];
	swprintf_s(text, 255,
		L"$%d",
		m_score[3]
	);
	m_scoreFontRender.SetText(text);
	m_scoreFontRender.SetPosition(Vector3(425.0f, -225.0f, 0.0f));
	m_scoreFontRender.SetScale(2.0f);
	m_scoreFontRender.SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	m_scoreFontRender.SetShadowParam(true, 0.5f, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Result::InitSprite()
{
	int lightNum = 0;

	//�w�i�摜�̐ݒ�
	m_backGroundSpriteRender.Init("Assets/sprite/UI/result/base.DDS", 1920.0f, 1080.0f);

	//�{�^���̐ݒ�
	m_cursorSpriteRender.Init("Assets/sprite/UI/title/button.DDS", 640.0f, 150.0f);
	m_cursorSpriteRender.SetPosition(Vector3(0.0f, -400.0f, 0.0f));
	m_cursorSpriteRender.Update();

	//���x���̃f�[�^���g�p���ă^�C�g���摜��ǂݍ���
	m_level2DRender.Init("Assets/level2D/result.casl", [&](Level2DObjectData& objData) {
		//���O����v���Ă�����B
		if (objData.EqualObjectName("missionComplete!") == true) {
			//�����摜��ݒ�
			if (m_resultState == enState_GameClear) {
				m_stateSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
				m_stateSpriteRender.SetPosition(objData.position);
			}
			else {
				m_stateSpriteRender.Init("Assets/sprite/UI/gameOver/gameover.DDS", 516.0f, 138.0f);
				m_stateSpriteRender.SetPosition(objData.position + GAMEOVER_SPRITE_POS);
			}
			m_stateSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("result_rank") == true) {
			//�����N�摜��ݒ�
			m_explainSpriteRender[0].Init(objData.ddsFilePath, objData.width, objData.height);
			m_explainSpriteRender[0].SetPosition(objData.position);

			Quaternion rot;
			rot.SetRotationZ(Math::DegToRad(-30.0f));
			//���s���̉摜��ݒ�
			m_failedSpriteRender.Init("Assets/sprite/UI/result/failed!.DDS", 497.0f, 170.0f);
			m_failedSpriteRender.SetPosition(objData.position);
			m_failedSpriteRender.SetRotation(rot);
			m_failedSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("result_text") == true) {
			//�X�R�A�摜��ݒ�
			m_explainSpriteRender[1].Init(objData.ddsFilePath, objData.width, objData.height);
			m_explainSpriteRender[1].SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualObjectName("light") == true) {
			//�X�R�A��\������ʒu��ݒ�
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
			NewGO<Title>(0, "title");
			DeleteGO(this);
		}
		return;
	}
	else {
		Input();
	}

	//���Ԃ̏���
	m_timer += g_gameTime->GetFrameDeltaTime();

	GameClear();
}

void Result::Input()
{
	//A�{�^������������
	if (g_pad[0]->IsTrigger(enButtonA)) {

		//�\�����S�Ċ������Ă��Ȃ�
		if (m_timer < ENABLE_TIME[4]) {
			//�\�����X�L�b�v����
			m_timer = ENABLE_TIME[4];
		}
		else {
			//�t�F�[�h���n�߂�
			m_isWaitFadeOut = true;
			m_fade->StartFadeOut();
		}
	}
}

void Result::GameClear()
{
	float time = m_timer - ENABLE_TIME[4];

	if (time > 2.0f) {
		m_timer = ENABLE_TIME[4];
	}

	// -t^2 + 2t
	m_alpha = fabsf(-pow((time - 0.5f), 1.0f) + (2 * (time - 0.5f)));
	m_alpha = min(m_alpha, 1.0f);

	//�{�^����_�ł�����
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_cursorSpriteRender.Update();

	if (m_timer > 5.0f && m_timer < 6.0f && m_cursor == 0) {
		//�����N��\������
		m_rankSpriteRender.SetScale(Vector3(1.0f, 1.0f, 1.0f) * (7.0f - m_timer));
		m_rankSpriteRender.Update();

		if (m_timer > 5.9f)
			m_cursor = 1;
	}
}

void Result::GameOver()
{
	if (m_timer > 1.0f)
		m_timer = -0.5f;

	// -t^2 + 2t
	m_alpha = fabsf(-pow(m_timer, 2.0f) + (2 * m_timer));
	m_alpha = min(m_alpha, 1.0f);
	m_alpha *= 3.0f;
	m_alpha = max(m_alpha, 1.0f);

	//�J�[�\����_�ł�����
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

	for (int i = 0; i < 4; i++) {
		if (m_timer > ENABLE_TIME[i])
			m_messageFontRender[i].Draw(rc);
	}

	if (m_timer >= ENABLE_TIME[4]) {
		m_explainSpriteRender[0].Draw(rc);
		m_scoreFontRender.Draw(rc);
		m_rankSpriteRender.Draw(rc);
		m_cursorSpriteRender.Draw(rc);

		if(m_resultState == enState_GameOver)
			m_failedSpriteRender.Draw(rc);
	}

	m_explainSpriteRender[1].Draw(rc);
}
