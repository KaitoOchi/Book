#pragma once
#include "GameManager.h"
#include "level2DRender/Level2DRender.h"

class Fade;

class Title :public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext &rc);

private:

	/// <summary>
	/// �X�v���C�g�̏����������B
	/// </summary>
	void InitSprite();

	/// <summary>
	/// ���͏����B
	/// </summary>
	void Input();

	/// <summary>
	/// ���͂ɂ��l�̍X�V�����B
	/// </summary>
	void ValueUpdate(bool vertical);

	/// <summary>
	/// �A�j���[�V���������B
	/// </summary>
	void Animation();

	/// <summary>
	/// �X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ManageState();

	/// <summary>
	/// �^�C�g����ʂ̏����B
	/// </summary>
	void TitleScreen();

	/// <summary>
	/// ���j���[��ʂ̏����B
	/// </summary>
	void MenuScreen();

	/// <summary>
	/// �Q�[���X�^�[�g���̏����B
	/// </summary>
	void StartScreen();

	/// <summary>
	/// �ݒ��ʂ̏����B
	/// </summary>
	void SettingScreen();

	/// <summary>
	/// �f�[�^�z��ɃZ�[�u�f�[�^�����ĕۑ����鏈���B
	/// </summary>
	void SetDataArray()
	{
		m_saveDataArray[0] = m_saveData.bgm * 100;
		m_saveDataArray[1] = m_saveData.sfx * 100;

		float frame = m_saveData.frameRate - 60.0f;
		for (int i = 0; i < 2; i++) {
			frame -= 30.0f;

			if (frame < 0) {
				m_saveDataArray[2] = i;
				break;
			}
		}
	}

	/// <summary>
	/// �Z�[�u�f�[�^�Ƀf�[�^�z������ĕۑ����鏈���B
	/// </summary>
	void SetSaveData()
	{
		m_saveData.bgm = (float)m_saveDataArray[0] / 100.0f;
		m_saveData.sfx = (float)m_saveDataArray[1] / 100.0f;
		m_saveData.frameRate = 60 + (m_saveDataArray[2] * 30);
		GameManager::GetInstance()->DataSave(m_saveData);
	}

private:
	SpriteRender	m_backGroundSpriteRender;		//�w�i�摜
	SpriteRender	m_titleSpriteRender;			//�^�C�g���摜
	SpriteRender	m_pressSpriteRender;			//press�摜
	SpriteRender	m_menuSpriteRender[3];			//���j���[�摜
	SpriteRender	m_guideSpriteRender;			//������@�摜

	SpriteRender	m_bgmSpriteRender;			//BGM�̃��[�^�[
	SpriteRender	m_sfxSpriteRender;			//SFX�̃��[�^�[
	FontRender		m_frameFontRender;				//�t���[�����[�g�̕\��

	SpriteRender	m_cursorSpriteRender;		//�J�[�\���摜

	FontRender		m_settingFontRender;

	FontRender		m_debugFontRender;

	Level2DRender* m_level2DRender = nullptr;

	GameManager::SaveData m_saveData;

	Fade* m_fade = nullptr;

	bool m_isWaitFadeOut = false;

	int				m_titleState = 0;			//�^�C�g���X�e�[�g
	int				m_cursor_vertical = 0;		//�c�J�[�\��
	int				m_cursor_horizontal = 0;	//���J�[�\��

	int m_saveDataArray[4];		//�Z�[�u�f�[�^�̈ꎞ�I�Ȕz��

	float m_alpha = 0.0f;		//�F�̃A���t�@�l
	float m_timer = 0.0f;		//����
};
