#include "stdafx.h"
#include "Title.h"

#include "GameManager.h"
#include "Fade.h"
#include "Game.h"

//#include "sound/SoundSource.h"
//#include "sound/SoundEngine.h"

namespace
{
	const int CURSOR_VERTICAL_MAX[5] = { 0, 3, 2, 0, 4 };			//�e�X�e�[�g�̏c�J�[�\���ő�l
	const int CURSOR_HORIZONTAL_MAX[10] = { 0, 100, 100, 2, 1 };	//�e�ݒ�̉��J�[�\���ő�l
}


Title::Title()
{

}

Title::~Title()
{

}

bool Title::Start()
{
	InitSprite();

	m_settingFontRender.SetText(L"BGM\n\n\n\nSFX\n\n\n\nFrame_Rate\n\n\n\nSETTING_B");
	m_settingFontRender.SetPosition(Vector3(-300.0f, 200.0f, 0.0f));

	m_frameFontRender.SetPosition(Vector3(0.0f, -150.0f, 0.0f));

	m_debugFontRender.SetPosition(Vector3(500.0f, 200.0f, 0.0f));

	//�Z�[�u�f�[�^�̃��[�h
	m_saveData = GameManager::GetInstance()->DataLoad();
	SetDataArray();

	//�t�F�[�h�̏���
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	//���莞�̉�
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/shot.wav");
	//�L�����Z�����̉�
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/shot.wav");

	return true;
}

void Title::InitSprite()
{
	//�w�i��ݒ�
	m_backGroundSpriteRender.Init("Assets/sprite/UI/title/base.DDS", 1728.0f, 972.0f);

	m_level2DRender = new Level2DRender;

	//���x���̃f�[�^���g�p���ă^�C�g���摜��ǂݍ��ށB
	m_level2DRender->Init("Assets/level2D/titleLevel.casl", [&](Level2DObjectData& objData) {
		//���O����v���Ă�����B
		if (objData.EqualObjectName("title") == true) {
			//�^�C�g����ݒ�
			m_titleSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_titleSpriteRender.SetPosition(objData.position);
			m_titleSpriteRender.SetScale(objData.scale);
			m_titleSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("button") == true) {
			//�{�^����ݒ�
			m_pressSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_pressSpriteRender.SetPosition(objData.position);
			m_pressSpriteRender.SetScale(objData.scale);
			return true;
		}
		return false;
	});
	delete m_level2DRender;
	m_level2DRender = new Level2DRender;

	//���x���̃f�[�^���g�p���ă��j���[�摜��ǂݍ��ށB
	m_level2DRender->Init("Assets/level2D/menuLevel.casl", [&](Level2DObjectData& objData) {
		//���O����v���Ă�����B
		if (objData.EqualObjectName("gameStart") == true) {
			//�Q�[���X�^�[�g��ݒ�
			m_menuSpriteRender[0].Init(objData.ddsFilePath, objData.width, objData.height);
			return true;
		}
		else if (objData.EqualObjectName("guide") == true) {
			//�K�C�h��ݒ�
			m_menuSpriteRender[1].Init(objData.ddsFilePath, objData.width, objData.height);
			return true;
		}
		else if (objData.EqualObjectName("setting") == true) {
			//�Z�b�e�B���O��ݒ�
			m_menuSpriteRender[2].Init(objData.ddsFilePath, objData.width, objData.height);
			return true;
		}
		return false;
	});

	for (int i = 0; i < 3; i++) {
		m_menuSpriteRender[i].SetPosition(Vector3(-650.0f, 50.0f + (-150.0f * i), 0.0f));
		m_menuSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_menuSpriteRender[i].Update();
	}
	delete m_level2DRender;

	//�K�C�h��ʂ̐ݒ�
	m_guideSpriteRender.Init("Assets/sprite/UI/guide/guide_add.DDS", 1920.0f, 1080.0f);
	m_guideSpriteRender.SetScale(Vector3(0.9f, 0.9f, 0.0f));
	m_guideSpriteRender.Update();

	//BGM���ʂ̉摜��ݒ�
	m_bgmSpriteRender.Init("Assets/sprite/UI/cautionTimeGauge/gaugeCount.DDS", 10.0f, 50.0f);
	m_bgmSpriteRender.SetPosition(Vector3(50.0f, 125.0f, 0.0f));
	m_bgmSpriteRender.SetPivot(Vector2(0.0f, 0.5f));
	m_bgmSpriteRender.Update();

	//SFX���ʂ̉摜��ݒ�
	m_sfxSpriteRender.Init("Assets/sprite/UI/cautionTimeGauge/gaugeCount.DDS", 10.0f, 50.0f);
	m_sfxSpriteRender.SetPosition(Vector3(50.0f, 0.0f, 0.0f));
	m_sfxSpriteRender.SetPivot(Vector2(0.0f, 0.5f));
	m_sfxSpriteRender.Update();

	//�J�[�\���摜�̐ݒ�
	m_cursorSpriteRender.Init("Assets/sprite/UI/title/tryangle.DDS", 131.0f, 135.0f);
}

void Title::Update()
{
	//���͏���
	Input();

	//�A�j���[�V��������
	Animation();

	//�X�e�[�g�̑J�ڏ���
	ManageState();


	m_cursorSpriteRender.Update();


	wchar_t debugText[255];
	swprintf_s(debugText, 255,
		L"STATE:%d \nCURSOR_V:%d \nCURSOR_H:%d \nSA0:%f \nSA1:%f \nalpha:%.2f",
		m_titleState,
		m_cursor_vertical,
		m_cursor_horizontal,
		m_saveDataArray[0],
		m_saveDataArray[1],
		m_alpha
	);
	m_debugFontRender.SetText(debugText);
}

void Title::Input()
{
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		if (m_titleState <= 1) {

			m_titleState = m_cursor_vertical + 1;
			m_cursor_vertical = 1;
			ValueUpdate(true);

			IsCanPlaySound(true);
		}
	}
	//B�{�^���������ꂽ��
	else if (g_pad[0]->IsTrigger(enButtonB))
	{
		//�ݒ��ʂȂ�
		if (m_titleState == 4) {
			//�ۑ����ĕ���
			SetSaveData();
			SetDataArray();
		}

		//���j���[��ʈȍ~�Ȃ�
		if (m_titleState >= 2) {
			m_titleState = 1;
			m_cursor_vertical = 1;
		}
		else {
			m_titleState--;
			m_cursor_vertical = 0;
		}

		IsCanPlaySound(false);
	}

	//��{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor_vertical--;
		ValueUpdate(true);
	}
	//���{�^���������ꂽ��
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor_vertical++;
		ValueUpdate(true);
	}

	//�ݒ��ʂȂ�
	if (m_titleState == 4) {

		//BGM�ASFX�ݒ�Ȃ�
		if (m_cursor_vertical == 1 || m_cursor_vertical == 2) {
			//���{�^���������ꂽ��
			if (g_pad[0]->IsPress(enButtonLeft)) {
				m_cursor_horizontal--;
				ValueUpdate(false);
			}
			//�E�{�^���������ꂽ��
			else if (g_pad[0]->IsPress(enButtonRight)) {
				m_cursor_horizontal++;
				ValueUpdate(false);
			}
		}
		else {
			//���{�^���������ꂽ��
			if (g_pad[0]->IsTrigger(enButtonLeft)) {
				m_cursor_horizontal--;
				ValueUpdate(false);
			}
			//�E�{�^���������ꂽ��
			else if (g_pad[0]->IsTrigger(enButtonRight)) {
				m_cursor_horizontal++;
				ValueUpdate(false);
			}
		}
	}
}

void Title::ValueUpdate(bool vertical)
{
	int cursor_v = m_cursor_vertical;
	int cursor_h = m_cursor_horizontal;

	//�͈͊O�ɂ͂ݏo���Ȃ��悤�ɂ���
	m_cursor_vertical = min(max(m_cursor_vertical, 1), CURSOR_VERTICAL_MAX[m_titleState]);
	m_cursor_horizontal = min(max(m_cursor_horizontal, 0), CURSOR_HORIZONTAL_MAX[m_cursor_vertical]);

	//�Q�[���X�^�[�g��ʂȂ�
	if (m_titleState == 2) {

	}
	//�ݒ��ʂȂ�
	else if (m_titleState == 4) {

		//����炷
		if (m_cursor_vertical == cursor_v || 
			m_cursor_horizontal == cursor_h)
		{
			Sound(2);
		}

		//���ێ����Ă���ݒ�̒l�Ɉړ�����
		if (vertical) {
			m_cursor_horizontal = m_saveDataArray[m_cursor_vertical - 1];
		}
		//�z��ɒl��ۑ�����
		else {
			m_saveDataArray[m_cursor_vertical - 1] = m_cursor_horizontal;
		}
	}
}

void Title::Animation()
{
	//���Ԃ̏���
	m_timer += g_gameTime->GetFrameDeltaTime();
	if (m_timer > 1.0f)
		m_timer = -0.5f;

	// -t^2 + 2t
	m_alpha = fabsf(-pow(m_timer, 2.0f) + (2 * m_timer));
	m_alpha = min(m_alpha, 1.0f);

	if (m_titleState == 1 || m_titleState == 4) {
		m_alpha *= 3.0f;
		m_alpha = max(m_alpha, 1.0f);
	}

	//�����x��ύX
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
}

void Title::ManageState()
{
	switch (m_titleState)
	{
	//�^�C�g����ʂȂ�
	case 0:
		TitleScreen();
		break;

	//���j���[��ʂȂ�
	case 1:
		MenuScreen();
		break;

	//�Q�[���X�^�[�g�Ȃ�
	case 2:
		StartScreen();
		break;

	//������@��ʂȂ�
	case 3:
		break;

	//�ݒ��ʂȂ�
	case 4:
		SettingScreen();
		break;
	}
}

void Title::TitleScreen()
{
	//�����x��ύX
	m_pressSpriteRender.SetMulColor(Vector4(m_alpha, m_alpha, m_alpha, m_alpha));
	m_pressSpriteRender.Update();
}

void Title::MenuScreen()
{
	m_cursorSpriteRender.SetPosition(Vector3(-700.0f, m_menuSpriteRender[m_cursor_vertical - 1].GetPosition().y, 0.0f));
}

void Title::StartScreen()
{
	//�t�F�[�h�̑ҋ@����
	if (m_isWaitFadeOut) {
		//�t�F�[�h���I������
		if (!m_fade->IsFade()) {
			//�Q�[����ʂ֑J��
			NewGO<Game>(0, "game");
			DeleteGO(this);
		}
	}
	else {
		m_isWaitFadeOut = true;
		m_fade->StartFadeOut();
	}
}

void Title::SettingScreen()
{
	int frame = 60 + (m_saveDataArray[2] * 30);

	wchar_t m_frameText[255];
	swprintf_s(m_frameText, 255,
		L"%d",
		frame
	);
	m_frameFontRender.SetText(m_frameText);

	m_cursorSpriteRender.SetPosition(Vector3(-300.0f, 300.0f + (-150.0f * m_cursor_vertical), 0.0f));

	m_bgmSpriteRender.SetScale(Vector3(m_saveDataArray[0], 1.0f, 0.0f));
	m_bgmSpriteRender.Update();
	m_sfxSpriteRender.SetScale(Vector3(m_saveDataArray[1], 1.0f, 0.0f));
	m_sfxSpriteRender.Update();
}

void Title::Render(RenderContext &rc)
{
	m_backGroundSpriteRender.Draw(rc);

	switch (m_titleState)
	{
	//�^�C�g����ʂȂ�
	case 0:
		m_pressSpriteRender.Draw(rc);
		m_titleSpriteRender.Draw(rc);
		break;

	//���j���[��ʂȂ�
	case 1:
		for (int i = 0; i < 3; i++) {
			m_menuSpriteRender[i].Draw(rc);
		}
		m_cursorSpriteRender.Draw(rc);
		break;

	//�Q�[���X�^�[�g��ʂȂ�
	case 2:
		break;

	//������@��ʂȂ�
	case 3:
		m_guideSpriteRender.Draw(rc);
		break;

	//�ݒ��ʂȂ�
	case 4:
		m_settingFontRender.Draw(rc);
		m_cursorSpriteRender.Draw(rc);

		m_bgmSpriteRender.Draw(rc);
		m_sfxSpriteRender.Draw(rc);

		m_frameFontRender.Draw(rc);
		break;
	}

	m_debugFontRender.Draw(rc);
}