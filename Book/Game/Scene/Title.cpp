#include "stdafx.h"
#include "Title.h"

#include "GameManager.h"


namespace
{
	const int CURSOR_VERTICAL_MAX[5] = { 0, 3, 2, 0, 4 };		//�e�X�e�[�g�̏c�J�[�\���ő�l
	const int CURSOR_HORIZONTAL_MAX[10] = { 0, 10, 10, 2, 2 };	//�e�ݒ�̉��J�[�\���ő�l
}


Title::Title()
{

}

Title::~Title()
{

}

bool Title::Start()
{
	for (int i = 0; i < 10; i++) {
		//BGM���ʂ̉摜��ݒ�
		m_bgmSpriteRender[i].Init("Assets/sprite/UI/cautionTimeGauge/gaugeCount.DDS", 83.0f, 100.0f);
		m_bgmSpriteRender[i].SetPosition(Vector3(50.0f * i, 300.0f, 0.0f));
		m_bgmSpriteRender[i].Update();

		//SFX���ʂ̉摜��ݒ�
		m_sfxSpriteRender[i].Init("Assets/sprite/UI/cautionTimeGauge/gaugeCount.DDS", 83.0f, 100.0f);
		m_sfxSpriteRender[i].SetPosition(Vector3(50.0f * i, 100.0f, 0.0f));
		m_sfxSpriteRender[i].Update();
	}

	//���x�����\�z����B
	//���x���̃f�[�^���g�p���ĉ摜��ǂݍ��ށB
	m_level2DRender.Init("Assets/level2D/titleLevel.casl", [&](Level2DObjectData& objData) {
		//���O����v���Ă�����B
		if (objData.EqualObjectName("base") == true) {
			//�w�i��ݒ�
			m_backGroundSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_backGroundSpriteRender.SetScale(objData.scale);
			m_backGroundSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("title") == true) {
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
	
	//�w�i�̐ݒ�
	m_backGroundSpriteRender.Init("Assets/sprite/UI/title/base.DDS", 1920.0f, 1080.0f);

	m_menuFontRender.SetText(L"Game Start\n\nGUIDE\n\nSetting");
	m_menuFontRender.SetPosition(Vector3(-600.0f, 0.0f, 0.0f));

	m_settingFontRender.SetText(L"BGM\n\nSFX\n\nSETTING_A\n\nSETTING_B");
	m_settingFontRender.SetPosition(Vector3(-300.0f, 400.0f, 0.0f));

	m_cursorFontRender.SetText(L">");

	m_debugFontRender.SetPosition(Vector3(500.0f, 200.0f, 0.0f));

	//�Z�[�u�f�[�^�̃��[�h
	m_saveData = GameManager::GetInstance()->DataLoad();

	SetDataArray();

	return true;
}

void Title::Update()
{
	//���͏���
	Input();

	//�A�j���[�V��������
	Animation();

	//�X�e�[�g�̑J�ڏ���
	ManageState();


	wchar_t debugText[255];
	swprintf_s(debugText, 255,
		L"STATE:%d \nCURSOR_V:%d \nCURSOR_H:%d \nSA0:%f \nSA1:%f",
		m_titleState,
		m_cursor_vertical,
		m_cursor_horizontal,
		m_saveDataArray[0],
		m_saveDataArray[1]
	);
	m_debugFontRender.SetText(debugText);
}

void Title::Input()
{
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_titleState = m_cursor_vertical + 1;
		m_cursor_vertical = 0;
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

		if (m_titleState >= 2) {
			m_titleState = 1;
		}
		else {
			m_titleState--;
		}
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
	//���{�^���������ꂽ��
	else if (g_pad[0]->IsTrigger(enButtonLeft)) {
		m_cursor_horizontal--;
		ValueUpdate(false);
	}
	//�E�{�^���������ꂽ��
	else if (g_pad[0]->IsTrigger(enButtonRight)) {
		m_cursor_horizontal++;
		ValueUpdate(false);
	}

	//�͈͊O�ɂ͂ݏo���Ȃ��悤�ɂ���
	m_cursor_vertical = min(max(m_cursor_vertical, 1), CURSOR_VERTICAL_MAX[m_titleState]);
	m_cursor_horizontal = min(max(m_cursor_horizontal, 0), CURSOR_HORIZONTAL_MAX[m_cursor_vertical]);
}

void Title::ValueUpdate(bool vertical)
{
	//�Q�[���X�^�[�g��ʂȂ�
	if (m_titleState == 2) {

	}
	//�ݒ��ʂȂ�
	else if (m_titleState == 4) {

		if (vertical) {
			//���ێ����Ă���ݒ�̒l�Ɉړ�����
			float tmp = m_saveDataArray[m_cursor_vertical - 1] * 10;
			m_cursor_horizontal = (int)tmp;
		}
		else {
			//�z��ɒl��ۑ�����
			m_saveDataArray[m_cursor_vertical - 1] = m_cursor_horizontal / 10.0f;
		}
	}
}

void Title::Animation()
{
	//���Ԃ̏���
	m_timer += g_gameTime->GetFrameDeltaTime();
	if (m_timer > 1.0f)
		m_timer = -0.5f;

	// (-2 * t^3) + (3 * t^2)
	m_alpha = fabsf((-2 * pow(m_timer, 3.0f)) + (3 * pow(m_timer, 2.0f)));

	//�����x��ύX
	m_cursorFontRender.SetColor(Vector4(m_alpha, m_alpha, m_alpha, m_alpha));
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
	m_pressSpriteRender.SetMulColor(Vector4(m_alpha, m_alpha, m_alpha, m_alpha));
	m_pressSpriteRender.Update();
}

void Title::MenuScreen()
{
	m_cursorFontRender.SetPosition(Vector3(-700.0f, 150.0f + (100 * -m_cursor_vertical), 0.0f));
}

void Title::StartScreen()
{

}

void Title::SettingScreen()
{
	m_cursorFontRender.SetPosition(Vector3(-300.0f, 350.0f + (100 * -m_cursor_vertical), 0.0f));
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
		m_menuFontRender.Draw(rc);
		m_cursorFontRender.Draw(rc);
		break;

	//�Q�[���X�^�[�g��ʂȂ�
	case 2:
		break;

	//������@��ʂȂ�
	case 3:
		break;

	//�ݒ��ʂȂ�
	case 4:
		m_settingFontRender.Draw(rc);
		m_cursorFontRender.Draw(rc);

		for (int i = 0; i < m_saveDataArray[0] * 10; i++) {
			m_bgmSpriteRender[i].Draw(rc);
		}

		for (int i = 0; i < m_saveDataArray[1] * 10; i++) {
			m_sfxSpriteRender[i].Draw(rc);
		}
		break;
	}

	m_debugFontRender.Draw(rc);
}