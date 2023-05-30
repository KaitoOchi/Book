#pragma once
#include "GameManager.h"
#include "Title.h"

class Fade;

class Title_Setting : public IGameObject
{
public:
	Title_Setting();
	~Title_Setting();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// �^�C�g���N���X�̃|�C���^��ݒ�B
	/// </summary>
	void SetTitlePtr(Title* title)
	{
		m_title = title;
	}

private:
	/// <summary>
	/// ���͏����B
	/// </summary>
	void Input();

	/// <summary>
	/// �J�[�\���̈ړ������B
	/// </summary>
	void CursorMove();

	/// <summary>
	/// ���͂ɂ��l�̍X�V�����B
	/// </summary>
	void ValueUpdate(const bool vertical, const bool vol, const bool setFPS);

	/// <summary>
	/// �R�}���h�����B
	/// </summary>
	void Command(char command);

	/// <summary>
	/// �摜�̍X�V�����B
	/// </summary>
	void SpriteUpdate();

	/// <summary>
	/// �X�e�[�g�̑J�ڏ����B
	/// </summary>
	void StateChange();

private:
	/// <summary>
	/// �f�[�^�z��ɃZ�[�u�f�[�^�����ĕۑ����鏈���B
	/// </summary>
	void SetDataArray()
	{
		m_saveDataArray[0] = m_saveData.bgm * 100;
		m_saveDataArray[1] = m_saveData.sfx * 100;

		float frame = m_saveData.frameRate - 60.0f;
		for (int i = 0; i < 3; i++) {
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
	/// �Z�[�u�f�[�^�����Z�b�g���鏈���B
	/// </summary>
	void ResetSaveData()
	{
		//SaveData�\���̂Ƀf�t�H���g�̒l�����ăZ�[�u
		GameManager::SaveData defaultSaveData;
		m_saveData = defaultSaveData;
		GameManager::GetInstance()->DataSave(m_saveData);

		//�z��ɏ����������Z�[�u�f�[�^������
		SetDataArray();

		//�l�ɉ����ĉ��ʂƉ摜��ݒ�
		GameManager::GetInstance()->SetVolume();
		SpriteUpdate();
		m_cursor_horizontal = 0;
		m_fpsCursorSpriteRender.SetPosition(Vector3(-120.0f, -240.0f, 0.0f));
		m_fpsCursorSpriteRender.Update();

		m_title->Sound(1);
	}

private:
	SpriteRender					m_settingSpriteRender;			//�ݒ�摜
	SpriteRender					m_cursorSpriteRender;			//�J�[�\���摜
	SpriteRender					m_fpsCursorSpriteRender;		//FPS�J�[�\��
	SpriteRender					m_catSpriteRender;				//�L�摜
	std::array< SpriteRender, 2 >	m_gaugeSpriteRender;			//BGM�̃��[�^�[
	std::array< SpriteRender, 3 >	m_settingTextSpriteRender;		//�ݒ�̐����摜
	std::array< SpriteRender, 3 >	m_buttonSpriteRender;			//�{�^���摜
	std::vector<SpriteRender*>		m_sprites;						//SpriteRender�̃x�N�^�[�^
	FontRender						m_percentFontRender;			//�p�[�Z���g����
	Fade* m_fade = nullptr;
	Title* m_title = nullptr;
	GameManager::SaveData			m_saveData;						//�Z�[�u�f�[�^�̍\����
	std::array< int, 3 >			m_saveDataArray;				//�Z�[�u�f�[�^�̈ꎞ�I�Ȕz��
	Vector3							m_cursorPos;					//�J�[�\�����W
	char							m_commandList[16] = {"NNNNNNNNNNNNNN0"};	//�R�}���h���X�g
	bool							m_isWaitState = false;			//�X�e�[�g�̑J�ڑ҂����ǂ���
	bool							m_isWaitFadeOut = false;		//�t�F�[�h�����ǂ���
	bool							m_isSetFPS = false;				//FPS���Z�b�g���邩
	int								m_cursor_vertical = 0;			//�c�J�[�\��
	int								m_cursor_horizontal = 0;		//���J�[�\��
	int								m_nextCursor = 1;				//���Ɉړ�����J�[�\��
	float							m_timer = 0.0f;					//����
	float							m_alpha = 0.0f;					//�����x
	float							m_cursorTimer = 1.0f;			//�J�[�\���^�C�}�[
};