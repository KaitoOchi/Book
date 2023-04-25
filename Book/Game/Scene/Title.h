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
	/// �X�e�[�g�̑J�ڒ��̏����B
	/// </summary>
	void StateChange();

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

	/// <summary>
	/// ���ʉ���点�邩�ǂ����B
	/// </summary>
	/// <param name="sound">�炵�������ʉ��̔ԍ��B</param>
	void IsCanPlaySound(const bool sound)
	{
		int title = m_titleState_tmp;
		title = min(max(m_titleState, 0), 4);

		if (m_titleState == title) {
			Sound(sound);
		}
	}

	/// <summary>
	/// ���ʉ���炷�����B
	/// </summary>
	/// <param name="num">�炵�������ʉ��̔ԍ��B</param>
	void Sound(const int num)
	{
		/*

		//�ˌ������o��
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());

		*/
	}

private:
	SpriteRender			m_backGroundSpriteRender;		//�w�i�摜
	SpriteRender			m_titleSpriteRender;			//�^�C�g���摜
	SpriteRender			m_pressSpriteRender;			//press�摜
	SpriteRender			m_menuSpriteRender;				//���j���[�摜
	SpriteRender			m_guideSpriteRender;			//������@�摜
	SpriteRender			m_settingSpriteRender;			//�ݒ�摜
	SpriteRender			m_gaugeSpriteRender[2];			//BGM�̃��[�^�[
	SpriteRender			m_settingTextSpriteRender[3];	//�ݒ�̐����摜
	SpriteRender			m_cursorSpriteRender;			//�J�[�\���摜
	SpriteRender			m_buttonSpriteRender[2];		//�{�^���摜

	std::vector<SpriteRender*> m_sprites;					//SpriteRender�̃x�N�^�[�^

	FontRender				m_debugFontRender;

	Level2DRender*			m_level2DRender = nullptr;		//���x�������_�[
	Fade*					m_fade = nullptr;				//�t�F�[�h�N���X
	GameManager::SaveData	m_saveData;
	bool					m_isWaitState = false;			//�X�e�[�g�̑J�ڒ����ǂ���
	bool					m_isWaitFadeOut = false;		//�t�F�[�h��Ԃ��ǂ���
	int						m_titleState = 0;				//�^�C�g���X�e�[�g
	int						m_titleState_tmp = 0;			//�^�C�g���X�e�[�g�̈ꎞ�I�ϐ�
	int						m_cursor_vertical = 0;			//�c�J�[�\��
	int						m_cursor_horizontal = 0;		//���J�[�\��
	int						m_saveDataArray[4];				//�Z�[�u�f�[�^�̈ꎞ�I�Ȕz��
	float					m_alpha = 0.0f;					//�F�̃A���t�@�l
	float					m_timer = 0.0f;					//����
	float					m_animTime = 1.0f;				//�A�j���[�V��������

	Vector3 a;
};
