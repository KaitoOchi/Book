#pragma once
#include "GameManager.h"
#include "level2DRender/Level2DRender.h"


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
		m_saveDataArray[0] = m_saveData.bgm;
		m_saveDataArray[1] = m_saveData.sfx;
	}

	/// <summary>
	/// �Z�[�u�f�[�^�Ƀf�[�^�z������ĕۑ����鏈���B
	/// </summary>
	void SetSaveData()
	{
		m_saveData.bgm = m_saveDataArray[0];
		m_saveData.sfx = m_saveDataArray[1];
		GameManager::GetInstance()->DataSave(m_saveData);
	}

private:
	SpriteRender	m_backGroundSpriteRender;
	SpriteRender	m_titleSpriteRender;
	SpriteRender	m_pressSpriteRender;
	SpriteRender	m_bgmSpriteRender[10];
	SpriteRender	m_sfxSpriteRender[10];
	FontRender		m_menuFontRender;
	FontRender		m_settingFontRender;
	FontRender		m_cursorFontRender;

	FontRender		m_debugFontRender;

	Level2DRender m_level2DRender;

	GameManager::SaveData m_saveData;

	int				m_titleState = 0;			//�^�C�g���X�e�[�g
	int				m_cursor_vertical = 0;		//�c�J�[�\��
	int				m_cursor_horizontal = 0;	//���J�[�\��

	float m_alpha = 0.0f;		//�F�̃A���t�@�l
	float m_timer = 0.0f;		//����
	float m_saveDataArray[4];
};
