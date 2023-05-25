#include "stdafx.h"
#include "Title_Setting.h"

#include "GameManager.h"
#include "Fade.h"
#include "Title.h"

namespace
{
	const int CURSOR_VERTICAL_MAX = 2;							//�c�J�[�\���ő�l
	const int CURSOR_HORIZONTAL_MAX[3] = { 100, 100, 2 };		//�e�ݒ�̉��J�[�\���ő�l
}

Title_Setting::Title_Setting()
{

}

Title_Setting::~Title_Setting()
{
	for (auto& sprites : m_sprites)
	{
		m_sprites.pop_back();
	}
	m_sprites.shrink_to_fit();
}

bool Title_Setting::Start()
{
	//�ݒ��ʂ̐ݒ�
	m_settingSpriteRender.Init("Assets/sprite/UI/title/setting_all.DDS", 1920.0f, 1080.0f);
	m_sprites.push_back(&m_settingSpriteRender);

	//�ݒ�e�L�X�g�̐ݒ�
	m_settingTextSpriteRender[0].Init("Assets/sprite/UI/setting/BGM_text.DDS", 443.0f, 31.0f);
	m_settingTextSpriteRender[1].Init("Assets/sprite/UI/setting/SE_text.DDS", 443.0f, 31.0f);
	m_settingTextSpriteRender[2].Init("Assets/sprite/UI/setting/FPS_text.DDS", 443.0f, 31.0f);
	for (int i = 0; i < 3; i++) {
		m_settingTextSpriteRender[i].SetPosition(Vector3(0.0f, -400.0f, 0.0f));
		m_settingTextSpriteRender[i].Update();
		m_sprites.push_back(&m_settingTextSpriteRender[i]);
	}

	//�J�[�\���摜�̐ݒ�
	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);
	m_sprites.push_back(&m_cursorSpriteRender);

	//�Q�[�W�摜�̐ݒ�
	m_gaugeSpriteRender[0].SetPosition(Vector3(-210.2, 166.4f, 0.0f));
	m_gaugeSpriteRender[1].SetPosition(Vector3(-211.3, -33.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Init("Assets/sprite/UI/setting/gauge.DDS", 750.0f, 67.0f, AlphaBlendMode_Trans, 2 + i);
		m_gaugeSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_gaugeSpriteRender[i].Update();
		m_sprites.push_back(&m_gaugeSpriteRender[i]);
	}

	//�{�^���摜�̐ݒ�
	m_buttonSpriteRender[0].Init("Assets/sprite/UI/button/text_Bbutton.DDS", 287.0f, 152.0f);
	m_buttonSpriteRender[1].Init("Assets/sprite/UI/button/text_crosskey.DDS", 198.0f, 133.0f);
	for (int i = 0; i < 2; i++) {
		m_buttonSpriteRender[i].SetPosition(Vector3(-825.0f, -375.0f - (i * 50.0f), 0.0f));
		m_buttonSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_buttonSpriteRender[i].SetScale(Vector3(0.75f, 0.75f, 0.0f));
		m_buttonSpriteRender[1].SetScale(Vector3(1.0f, 1.0f, 0.0f));
		m_buttonSpriteRender[i].Update();
		m_sprites.push_back(&m_buttonSpriteRender[i]);
	}

	//�Z�[�u�f�[�^�̃��[�h
	m_saveData = GameManager::GetInstance()->DataLoad();
	SetDataArray();
	m_cursor_horizontal = m_saveDataArray[m_cursor_vertical];

	SpriteUpdate();

	//�t�F�[�h�̏���
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Title_Setting::Update()
{
	//�t�F�[�h�̑ҋ@����
	if (m_isWaitState) {
		StateChange();
	}
	else {
		m_title->Animation(m_timer, m_alpha);
		m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));

		Input();
	}
}

void Title_Setting::Input()
{
	//B�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_timer = 1.0f;
		m_isWaitState = true;

		m_title->IsCanPlaySound(false);
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

	//BGM�ASFX�ݒ�Ȃ�
	if (m_cursor_vertical == 0 || m_cursor_vertical == 1) {
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
	//FPS�ݒ�Ȃ�
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

void Title_Setting::ValueUpdate(bool vertical)
{
	int cursor_v = m_cursor_vertical;
	int cursor_h = m_cursor_horizontal;

	//�͈͊O�ɂ͂ݏo���Ȃ��悤�ɂ���
	m_cursor_vertical = min(max(m_cursor_vertical, 0), CURSOR_VERTICAL_MAX);
	m_cursor_horizontal = min(max(m_cursor_horizontal, 0), CURSOR_HORIZONTAL_MAX[m_cursor_vertical]);

	//����炷
	if (m_cursor_vertical == cursor_v &&
		m_cursor_horizontal == cursor_h)
	{
		m_title->Sound(2);
	}

	//���ێ����Ă���ݒ�̒l�Ɉړ�����
	if (vertical) {
		m_cursor_horizontal = m_saveDataArray[m_cursor_vertical];
	}
	//�z��ɒl��ۑ�����
	else {
		m_saveDataArray[m_cursor_vertical] = m_cursor_horizontal;
		//�ۑ�����
		SetSaveData();
		GameManager::GetInstance()->SetVolume();

		m_title->Sound(2);
	}

	//�摜�̍X�V����
	SpriteUpdate();
}

void Title_Setting::SpriteUpdate()
{
	//FPS�̐ݒ�
	if (m_cursor_vertical == 2) {
		m_cursorSpriteRender.SetPosition(Vector3(-200.0f + (m_cursor_horizontal * 275.0f), -240.0f, 0.0f));
	}
	//���ʂ̐ݒ�
	else {
		m_cursorSpriteRender.SetPosition(Vector3(-600.0f, 165.0f + (-205.0f * m_cursor_vertical), 0.0f));
	}
	m_cursorSpriteRender.Update();

	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.x = 590.0f + (m_saveDataArray[0] * 7.5f);
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = 590.0f + (m_saveDataArray[1] * 7.5f);
	//BGM��SFX���ʂ̃Q�[�W��ύX
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Update();
	}
}

void Title_Setting::StateChange()
{
	if (m_isWaitFadeOut) {
		//�t�F�[�h���I������
		if (!m_fade->IsFade()) {
			//�^�C�g���N���X�֖߂�
			m_title->SetActive(true);
			DeleteGO(this);
		}
		return;
	}
	else {

		if (m_timer < 0.0f) {
			m_isWaitFadeOut = true;
			m_fade->StartFadeOut();
		}
		else {
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}
	}

	//�摜�̓����x��ύX
	for (auto& sprites : m_sprites)
	{
		sprites->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(m_timer)));
		sprites->Update();
	}
}

void Title_Setting::Render(RenderContext& rc)
{
	//�Q�[�W�摜�̕`��
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Draw(rc);
	}

	//�{�^���摜�̕`��
	for (int i = 0; i < 2; i++) {
		m_buttonSpriteRender[i].Draw(rc);
	}

	m_settingSpriteRender.Draw(rc);
	m_settingTextSpriteRender[m_cursor_vertical].Draw(rc);
	m_cursorSpriteRender.Draw(rc);
}