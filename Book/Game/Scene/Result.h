#pragma once
#include "level2DRender/Level2DRender.h"

class Fade;

class Result : public IGameObject
{
public:
	Result();
	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���U���g��ݒ�B
	/// </summary>
	/// <param name="isClear">�N���A�������ǂ����Bfalse�ŃQ�[���I�[�o�[�B</param>
	void SetResult(const bool isClear)
	{
		if (isClear)
			m_resultState = enState_GameClear;
		else
			m_resultState = enState_GameOver;
	}

private:

	/// <summary>
	/// �Q�[���N���A���̏����������B
	/// </summary>
	void InitGameClear();

	/// <summary>
	/// �Q�[���I�[�o�[���̏����������B
	/// </summary>
	void InitGameOver();

	/// <summary>
	/// ���͏����B
	/// </summary>
	void Input();

	/// <summary>
	/// �Q�[���N���A�����B
	/// </summary>
	void GameClear();

	/// <summary>
	/// �Q�[���I�[�o�[�����B
	/// </summary>
	void GameOver();

	/// <summary>
	/// �ҋ@���Ԃ��I���������ǂ���
	/// </summary>
	/// <returns></returns>
	const bool& IsEndAnimation()
	{
		if (m_canInputTime < 0.0f) {
			return true;
		}
		else {
			m_canInputTime -= g_gameTime->GetFrameDeltaTime();
			return false;
		}
	}

private:
	//���U���g�X�e�[�g
	enum ResultState
	{
		enState_GameClear,
		enState_GameOver
	};

private:
	SpriteRender	m_backGroundSpriteRender;				//�w�i�摜
	SpriteRender	m_stateSpriteRender;					//�X�e�[�g�����摜
	SpriteRender	m_explainSpriteRender[2];				//�ڍ׉摜
	SpriteRender	m_cursorSpriteRender;					//�J�[�\��
	FontRender		m_messageFontRender[4];					//�X�R�A���b�Z�[�W
	FontRender		m_rankFontRender;						//�����N����

	Level2DRender	m_level2DRender;						//���x��2D�����_�[

	ResultState		m_resultState = enState_GameClear;		//���U���g�X�e�[�g

	Fade*			m_fade = nullptr;						//�t�F�[�h�N���X

	Vector3			m_fontPosition[4];						//�������o�����W

	bool			m_isWaitFadeOut = false;				//�t�F�[�h���Ă��邩�ǂ���
	int				m_cursor = 0;							//�J�[�\��
	int				m_score[4];								//�N���A���̃X�R�A
	float			m_timer = 0.0f;							//�^�C�}�[
	float			m_alpha = 0.0f;							//�����x
	float			m_canInputTime = 0.0f;					//���͉\����
};

