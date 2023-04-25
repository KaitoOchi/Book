#pragma once

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
	/// <param name="vigilance">�x���x�B</param>
	/// <param name="clearTime">�N���A�^�C���B</param>
	void SetResult(const bool isClear = false, const int vigilance = 99, const float clearTime = 99.0f)
	{
		m_vigilance = vigilance;
		m_clearTime = clearTime;

		if (isClear)
			m_resultState = enState_GameClear;
		else
			m_resultState = enState_GameOver;
	}

private:
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
	FontRender		m_messageFontRender;					//���U���g���b�Z�[�W

	ResultState		m_resultState = enState_GameClear;		//���U���g�X�e�[�g

	Fade*			m_fade = nullptr;						//�t�F�[�h�N���X

	bool			m_isWaitFadeOut = false;				//�t�F�[�h���Ă��邩�ǂ���
	int				m_vigilance = 0;						//�x���x
	int				m_cursor = 0;							//�J�[�\��
	float			m_clearTime = 0.0f;						//�N���A�^�C��
	float			m_timer = 0.0f;
	float			m_alpha = 0.0f;
	float			m_canInputTime = 0.0f;					//���͉\����
};

