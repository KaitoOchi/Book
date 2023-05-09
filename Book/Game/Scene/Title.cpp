#include "stdafx.h"
#include "Title.h"

#include "GameManager.h"
#include "Fade.h"
#include "Game.h"

namespace
{
	const int CURSOR_VERTICAL_MAX[5] = { 0, 3, 2, 0, 3 };		//�e�X�e�[�g�̏c�J�[�\���ő�l
	const int CURSOR_HORIZONTAL_MAX[4] = { 0, 100, 100, 2};		//�e�ݒ�̉��J�[�\���ő�l
}

Title::Title()
{

}

Title::~Title()
{
	for (auto& sprites : m_sprites)
	{
		m_sprites.pop_back();
	}
}

bool Title::Start()
{
	InitSprite();

	//�J�����̐ݒ�
	g_camera3D->SetPosition({ 0.0f, 0.0f, -600.0f });
	g_camera3D->SetTarget({ 0.0f, 00.0f, 0.0f });
	g_camera3D->Update();

	//�Z�[�u�f�[�^�̃��[�h
	m_saveData = GameManager::GetInstance()->DataLoad();
	SetDataArray();

	//�t�F�[�h�̏���
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	//BGM��炷
	GameManager::GetInstance()->SetBGM(20);

	return true;
}

void Title::InitSprite()
{
	//�w�i��ݒ�
	m_backGroundModelRender.Init("Assets/modelData/title/title_model.tkm", 0, 0, enModelUpAxisZ, false, false, 0, D3D12_CULL_MODE_FRONT);
	m_backGroundModelRender.SetPosition(Vector3(0.0f, 0.0f, 335.0f));
	m_backGroundModelRender.Update();

	//�v���C���[���f����ݒ�
	m_animationClips[animationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[animationClip_Idle].SetLoopFlag(true);
	m_animationClips[animationClip_Put].Load("Assets/animData/player/idle_act.tka");
	m_animationClips[animationClip_Put].SetLoopFlag(false);

	Quaternion rot;
	rot.AddRotationY(Math::DegToRad(-120.0f));

	m_playerModelRender.Init("Assets/modelData/player/player.tkm", m_animationClips, animationClip_Num, enModelUpAxisZ);
	m_playerModelRender.SetPosition(Vector3(100.0f, -100.0f, -400.0f));
	m_playerModelRender.SetRotation(rot);
	m_playerModelRender.Update();


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
			m_sprites.push_back(&m_titleSpriteRender);
			return true;
		}
		else if (objData.EqualObjectName("button") == true) {
			//�{�^����ݒ�
			m_pressSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_pressSpriteRender.SetPosition(objData.position);
			m_pressSpriteRender.SetScale(objData.scale);
			m_sprites.push_back(&m_pressSpriteRender);
			return true;
		}
		return false;
	});

	m_menuSpriteRender.Init("Assets/sprite/UI/title/title_2_all.DDS", 1920.0f, 1080.0f);
	m_sprites.push_back(&m_menuSpriteRender);

	//�K�C�h�w�i��ʂ̐ݒ�
	m_guideBackSpriteRender.Init("Assets/sprite/UI/guide/guide_all2.DDS", 1536.0f, 891.0f);
	m_guideBackSpriteRender.SetPosition(Vector3(-50.0f, 35.0f, 0.0f));
	m_guideBackSpriteRender.Update();
	m_sprites.push_back(&m_guideBackSpriteRender);

	//�K�C�h��ʂ̐ݒ�
	m_guideSpriteRender[0].Init("Assets/sprite/UI/guide/guide_playguide.DDS", 1403.0f, 637.0f);
	m_guideSpriteRender[1].Init("Assets/sprite/UI/guide/rule.DDS", 1141.0f, 541.0f);

	for (int i = 0; i < 2; i++) {
		m_guideSpriteRender[i].SetPosition(Vector3(0.0f, -10.0f, 0.0f));
		m_guideSpriteRender[i].Update();
		m_sprites.push_back(&m_guideSpriteRender[i]);
	}



	//�ݒ��ʂ̐ݒ�
	m_settingSpriteRender.Init("Assets/sprite/UI/title/setting_all.DDS", 1920.0f, 1080.0f);
	m_sprites.push_back(&m_settingSpriteRender);

	//�ݒ�e�L�X�g�̐ݒ�
	m_settingTextSpriteRender[0].Init("Assets/sprite/UI/setting/BGM_text.DDS", 380.0f, 28.0f);
	m_settingTextSpriteRender[1].Init("Assets/sprite/UI/setting/SE_text.DDS", 227.0f, 28.0f);
	m_settingTextSpriteRender[2].Init("Assets/sprite/UI/setting/FPS_text.DDS", 457.0f, 27.0f);

	for (int i = 0; i < 3; i++) {
		m_settingTextSpriteRender[i].SetPosition(Vector3(0.0f, -400.0f, 0.0f));
		m_settingTextSpriteRender[i].Update();
		m_sprites.push_back(&m_settingTextSpriteRender[i]);
	}

	m_gaugeSpriteRender[0].SetPosition(Vector3(-210.2, 166.4f, 0.0f));
	m_gaugeSpriteRender[1].SetPosition(Vector3(-211.3, -36.6f, 0.0f));

	//BGM��SFX���ʂ̉摜��ݒ�
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Init("Assets/sprite/UI/setting/gauge.DDS", 750.0f, 67.0f, AlphaBlendMode_Trans, 2 + i);
		m_gaugeSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_gaugeSpriteRender[i].Update();
		m_sprites.push_back(&m_gaugeSpriteRender[i]);
	}

	//�J�[�\���摜�̐ݒ�
	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);
	m_sprites.push_back(&m_cursorSpriteRender);

	//�{�^���摜�̐ݒ�
	m_buttonSpriteRender[0].Init("Assets/sprite/UI/button/text_Abutton.DDS", 287.0f, 152.0f);
	m_buttonSpriteRender[1].Init("Assets/sprite/UI/button/text_Bbutton.DDS", 287.0f, 152.0f);
	m_buttonSpriteRender[2].Init("Assets/sprite/UI/button/text_crosskey.DDS", 198.0f, 133.0f);

	for (int i = 0; i < 3; i++) {
		m_buttonSpritePos[i] = { -825.0f, -375.0f - (i * 50.0f), 0.0f };
		m_buttonSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_buttonSpriteRender[i].SetScale(Vector3(0.75f, 0.75f, 0.0f));
		m_buttonSpriteRender[2].SetScale(Vector3(1.0f, 1.0f, 0.0f));
		m_buttonSpriteRender[i].Update();
		m_sprites.push_back(&m_buttonSpriteRender[i]);
	}
}

void Title::Update()
{
	//�X�e�[�g�̑J�ڏ���
	ManageState();

	if (m_isWaitState) {
		//�X�e�[�g�̑J�ڒ��̏����B
		StateChange();

		m_playerModelRender.SetAnimationSpeed(0.9);
		m_playerModelRender.PlayAnimation(animationClip_Put, 0.5f);
		m_playerModelRender.Update();
		return;

	}

	m_playerModelRender.PlayAnimation(animationClip_Idle, 0.5f);
	m_playerModelRender.Update();

	//���͏���
	Input();

	//�A�j���[�V��������
	Animation();
}

void Title::StateChange()
{
	//�ꎞ�I�ȕϐ��̒l������
	if (m_animTime < 0.1f && m_animTime > -0.1f) {

		//���j���[��ʈȍ~�Ȃ�
		if (m_titleState_tmp > 1 || m_titleState > 2) {

			//�t�F�[�h�A�E�g�̑ҋ@����
			if (m_isWaitFadeOut) {
				//�t�F�[�h�A�E�g���I������
				if (!m_fade->IsFade()) {
					//�t�F�[�h�C���̏���
					m_fade->StartFadeIn();
					m_isWaitFadeOut = false;
					m_titleState = m_titleState_tmp;
					m_animTime = -0.11f;
					EnableButtonSprite();
				}
			}
			else {
				m_isWaitFadeOut = true;
				m_fade->StartFadeOut();
			}
			return;
		}
		else {
			m_titleState = m_titleState_tmp;
			EnableButtonSprite();
		}
	}
	//�X�e�[�g�J�ڂ��I������
	else if (m_animTime < -1.0f) {

		m_isWaitState = false;
		m_animTime = 1.0f;
	}

	m_animTime -= g_gameTime->GetFrameDeltaTime();

	//�摜�̓����x��ύX
	for (auto& sprites : m_sprites)
	{
		sprites->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(m_animTime)));
		sprites->Update();
	}
}

void Title::Input()
{
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//�^�C�g���ƃ��j���[��ʂȂ�
		if (m_titleState_tmp <= 1) {

			m_titleState_tmp = m_cursor_vertical + 1;
			m_cursor_vertical = 1;
			ValueUpdate(true);

			//�摜�̃A�j���[�V�������s��
			m_isWaitState = true;

			IsCanPlaySound(true);
		}
		//������ʂȂ�
		else if (m_titleState_tmp == 3) {

			if (m_cursor_horizontal != 0) {
				m_titleState_tmp = 1;
				m_cursor_vertical = 1;
				m_isWaitState = true;
			}
			else {
				m_cursor_horizontal++;
			}
			IsCanPlaySound(true);
		}
	}
	//B�{�^���������ꂽ��
	else if (g_pad[0]->IsTrigger(enButtonB))
	{
		if (m_titleState_tmp == 0) {
			return;
		}

		//������ʂȂ�
		if (m_titleState_tmp == 3) {

			if (m_cursor_horizontal == 1) {
				m_cursor_horizontal = 0;
			}
			else {
				m_titleState_tmp = 1;
				m_cursor_vertical = 1;
				m_isWaitState = true;
			}
		}
		//���j���[��ʈȍ~�Ȃ�
		else if (m_titleState_tmp >= 2) {
			m_titleState_tmp = 1;
			m_cursor_vertical = 1;
			m_isWaitState = true;
		}
		else {
			m_titleState_tmp--;
			m_cursor_vertical = 0;
			m_isWaitState = true;
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
	if (m_titleState_tmp == 4) {

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
	m_cursor_vertical = min(max(m_cursor_vertical, 1), CURSOR_VERTICAL_MAX[m_titleState_tmp]);
	m_cursor_horizontal = min(max(m_cursor_horizontal, 0), CURSOR_HORIZONTAL_MAX[m_cursor_vertical]);

	//���j���[��ʂȂ�
	if (m_titleState_tmp == 1) {

		//����炷
		if (m_cursor_vertical == cursor_v &&
			m_cursor_horizontal == cursor_h)
		{
			Sound(2);
		}
	}
	//�ݒ��ʂȂ�
	else if (m_titleState_tmp == 4) {

		//����炷
		if (m_cursor_vertical == cursor_v &&
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
			//�ۑ�����
			SetSaveData();
			GameManager::GetInstance()->SetVolume();

			Sound(2);
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

	if (m_titleState_tmp == 1 || m_titleState_tmp == 4) {
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
		HowToScreen();
		break;

	//�ݒ��ʂȂ�
	case 4:
		SettingScreen();
		break;
	}
}

void Title::TitleScreen()
{
	m_titleSpriteRender.SetPosition(Vector3(-340.0f, 250.0f, 0.0f));
	m_titleSpriteRender.SetScale(Vector3(1.0f, 1.0f, 0.0f));
	m_titleSpriteRender.Update();

	//�����x��ύX
	m_pressSpriteRender.SetMulColor(Vector4(m_alpha, m_alpha, m_alpha, m_alpha));
	m_pressSpriteRender.Update();
}

void Title::MenuScreen()
{
	m_titleSpriteRender.SetPosition(Vector3(-675.0f, 400.0f, 0.0f));
	m_titleSpriteRender.SetScale(Vector3(0.25f, 0.25f, 0.0f));
	m_titleSpriteRender.Update();

	if (!m_isWaitState || m_animTime < 0.0f) {
		m_cursorSpriteRender.SetPosition(Vector3(-725.0f,  475.0f + (m_cursor_vertical * -235.0f), 0.0f));
		m_cursorSpriteRender.Update();
	}
}

void Title::StartScreen()
{
	//�t�F�[�h�̑ҋ@����
	if (m_isWaitFadeOut) {
		//�t�F�[�h���I������
		if (!m_fade->IsFade()) {

			//BGM���폜���ꂽ��
			if (GameManager::GetInstance()->IsDeleteBGM()) {
				//�Q�[����ʂ֑J��
				NewGO<Game>(0, "game");
				DeleteGO(this);
			}
		}
	}
	else {
		m_isWaitFadeOut = true;
		m_fade->StartFadeOut();

		//BGM���폜����
		GameManager::GetInstance()->DeleteBGM();
	}
}

void Title::HowToScreen()
{

}

void Title::SettingScreen()
{
	if (!m_isWaitState || m_animTime < 0.0f) {
		//FPS�̐ݒ�
		if (m_cursor_vertical == 3) {
			m_cursorSpriteRender.SetPosition(Vector3(-200.0f + (m_cursor_horizontal * 275.0f), -240.0f, 0.0f));
		}
		//���ʂ̐ݒ�
		else {
			m_cursorSpriteRender.SetPosition(Vector3(-600.0f, 370.0f + (-205.0f * m_cursor_vertical), 0.0f));
		}
		m_cursorSpriteRender.Update();
	}

	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.x = 590.0f + (m_saveDataArray[0] * 7.5f);
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = 590.0f + (m_saveDataArray[1] * 7.5f);
	//BGM��SFX���ʂ̃Q�[�W��ύX
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Update();
	}
}

void Title::EnableButtonSprite()
{
	//�{�^���̕\����Ԃ�ݒ�
	switch (m_titleState)
	{
	case 0:
		for (int i = 0; i < 3; i++) {
			m_enableButtonSprite[i] = false;
		}
		break;

	case 1:
		m_enableButtonSprite[0] = true;
		m_enableButtonSprite[1] = false;
		m_enableButtonSprite[2] = true;
		break;

	case 2:
		break;

	case 3:
		m_enableButtonSprite[0] = true;
		m_enableButtonSprite[1] = true;
		m_enableButtonSprite[2] = false;
		break;

	case 4:
		m_enableButtonSprite[0] = false;
		m_enableButtonSprite[1] = true;
		m_enableButtonSprite[2] = true;
		break;
	}

	int num = 0;

	//�{�^���̍��W��ݒ�
	for (int i = 0; i < 3; i++) {

		if (m_enableButtonSprite[i] == true) {
			m_buttonSpriteRender[i].SetPosition(m_buttonSpritePos[num]);
			num++;
		}
	}
}

void Title::Render(RenderContext &rc)
{
	m_backGroundModelRender.Draw(rc);

	switch (m_titleState)
	{
	//�^�C�g����ʂȂ�
	case 0:
		m_pressSpriteRender.Draw(rc);
		m_titleSpriteRender.Draw(rc);
		m_playerModelRender.Draw(rc);
		break;

	//���j���[��ʂȂ�
	case 1:
		m_menuSpriteRender.Draw(rc);
		m_titleSpriteRender.Draw(rc);
		m_playerModelRender.Draw(rc);
		m_cursorSpriteRender.Draw(rc);
		break;

	//�Q�[���X�^�[�g��ʂȂ�
	case 2:
		break;

	//������@��ʂȂ�
	case 3:
		m_guideBackSpriteRender.Draw(rc);
		m_guideSpriteRender[m_cursor_horizontal].Draw(rc);
		break;

	//�ݒ��ʂȂ�
	case 4:
		for (int i = 0; i < 2; i++) {
			m_gaugeSpriteRender[i].Draw(rc);
		}

		m_settingSpriteRender.Draw(rc);
		m_settingTextSpriteRender[m_cursor_vertical -1].Draw(rc);
		m_cursorSpriteRender.Draw(rc);
		break;
	}

	//�{�^���摜�̕`��
	for (int i = 0; i < 3; i++) {
		if (m_enableButtonSprite[i] == true) {
			m_buttonSpriteRender[i].Draw(rc);
		}
	}
}