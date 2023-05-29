#include "stdafx.h"
#include "Fade.h"

Fade::Fade()
{
	srand(time(0));
	m_spriteNum = rand() % 4;
}

Fade::~Fade()
{

}

bool Fade::Start()
{
	//�Ó]���̉摜
	m_spriteRender.Init("Assets/sprite/black.dds", 1920.0f, 1080.0f);

	//���[�f�B���O�����摜�̐ݒ�
	m_nowLoadingSpriteRender.Init("Assets/sprite/UI/tips/now_loading.DDS", 455.0f, 53.0f);
	m_nowLoadingSpriteRender.SetPosition(Vector3(550.0f, -425.0f, 0.0f));
	m_nowLoadingSpriteRender.Update();

	//���[�f�B���O�摜�̐ݒ�
	m_loadingSpriteRender.Init("Assets/sprite/UI/tips/loading.DDS", 148.0f, 148.0f);
	m_loadingSpriteRender.SetScale(Vector3(0.3f, 0.3f, 0.0f));
	m_loadingSpriteRender.SetPosition(Vector3(280.0f, -425.0f, 0.0f));
	m_loadingSpriteRender.Update();

	//tip�摜1�`15��ݒ�
	wchar_t path[255];
	char finalFilePath[100];
	for (int i = 1; i < TIPS_MAX + 1; i++) {
		
		swprintf_s(path, 255, L"Assets/sprite/UI/tips/%d.DDS", i);
		//wchar_t��char�ɕϊ�
		wcstombs(finalFilePath, path, sizeof(path));
		//tips�摜�̐ݒ�
		m_tipsSpriteRender[i - 1].Init(finalFilePath, 1178.0f, 755.0f);
	}
	return true;
}

void Fade::Update()
{
	switch (m_state)
	{
		//�t�F�[�h�C�����Ȃ�
	case enState_FadeIn:
		m_alpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_alpha <= 0.0f)
		{
			m_alpha = 0.0f;
			m_state = enState_Idle;
		}
		break;

		//�t�F�[�h�A�E�g���Ȃ�
	case enState_FadeOut:
		m_alpha += 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_alpha >= 2.0f)
		{
			if (m_enableTips) {
				m_alpha = 2.0f;
			}
			else {
				m_alpha = 1.0f;
			}
			m_state = enState_Idle;
		}
		break;

	case enState_Idle:
		break;
	}

	if (m_alpha < 0.0f) {
		return;
	}

	SpriteUpdate();
}

void Fade::SpriteUpdate()
{
	//�w�i�摜��ݒ�
	m_spriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
	m_spriteRender.Update();

	//tips�摜��ݒ�
	if (m_enableTips) {
		m_tipsSpriteRender[m_spriteNum].SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
		m_tipsSpriteRender[m_spriteNum].Update();

		//���[�f�B���O�����̐ݒ�
		m_nowLoadingSpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
		m_nowLoadingSpriteRender.Update();

		//���[�f�B���O�摜�̐ݒ�
		m_rotTimer += g_gameTime->GetFrameDeltaTime();
		Quaternion rot;
		rot.SetRotationZ(Math::DegToRad(m_rotTimer * -180.0f));
		m_loadingSpriteRender.SetRotation(rot);
		m_loadingSpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
		m_loadingSpriteRender.Update();
	}
	else {
		m_tipsSpriteRender[m_spriteNum].SetMulColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		m_tipsSpriteRender[m_spriteNum].Update();
	}
}

void Fade::Render(RenderContext& rc)
{
	if (m_alpha < 0.0f) {
		return;
	}

	//�w�i�摜�̕`��
	m_spriteRender.Draw(rc);

	if (m_enableTips) {

		//�q���g�摜�̕`��
		m_tipsSpriteRender[m_spriteNum].Draw(rc);

		//���[�f�B���O�����̕`��
		m_nowLoadingSpriteRender.Draw(rc);

		//���[�f�B���O�摜�̕`��
		m_loadingSpriteRender.Draw(rc);
	}
}