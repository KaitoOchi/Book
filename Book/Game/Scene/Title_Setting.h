#pragma once
#include "GameManager.h"

class Fade;
class Title;

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
	/// ���͂ɂ��l�̍X�V�����B
	/// </summary>
	void ValueUpdate(bool vertical);

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
	SpriteRender					m_settingSpriteRender;			//�ݒ�摜
	SpriteRender					m_cursorSpriteRender;			//�J�[�\���摜
	std::array< SpriteRender, 2 >	m_gaugeSpriteRender;			//BGM�̃��[�^�[
	std::array< SpriteRender, 3 >	m_settingTextSpriteRender;		//�ݒ�̐����摜
	std::array< SpriteRender, 2 >	m_buttonSpriteRender;			//�{�^���摜

	std::vector<SpriteRender*>		m_sprites;						//SpriteRender�̃x�N�^�[�^

	Fade* m_fade = nullptr;
	Title* m_title = nullptr;

	GameManager::SaveData			m_saveData;						//�Z�[�u�f�[�^�̍\����
	std::array< int, 4 >			m_saveDataArray;				//�Z�[�u�f�[�^�̈ꎞ�I�Ȕz��

	bool							m_isWaitState = false;			//�X�e�[�g�̑J�ڑ҂����ǂ���
	bool							m_isWaitFadeOut = false;		//�t�F�[�h�����ǂ���
	int								m_cursor_vertical = 0;			//�c�J�[�\��
	int								m_cursor_horizontal = 0;		//���J�[�\��
	float							m_timer = 0.0f;					//����
	float							m_alpha = 0.0f;					//�����x
};