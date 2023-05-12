#pragma once

class Fade;

class Event : public IGameObject
{
public:
	Event();
	~Event();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// ��̍��W��ݒ�B
	/// </summary>
	void SetTresurePosition(const Vector3& pos)
	{
		m_tresurePos = pos;
	}

private:
	/// <summary>
	/// ���ԏ����B
	/// </summary>
	void Time();

	/// <summary>
	/// �A�j���[�V���������B
	/// </summary>
	void Animation();

	/// <summary>
	/// �J�����̏����B
	/// </summary>
	void Camera();

	/// <summary>
	/// �A�j���[�V�����C�x���g�B
	/// </summary>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

private:
	enum EnAnimationClip {
		animationClip_Idle,
		animationClip_Walk,
		animationClip_HeadUp,
		animationClip_HeadStop,
		animationClip_HeadDown,
		animationClip_RunAway,
		animationClip_Num,
	};
	AnimationClip m_animationClips[animationClip_Num];

	enum EnEnemyAnimClip {
		enemyAnimClip_Run,
		enemyAnimClip_Num,
	};
	AnimationClip m_enemyAnimClips[enemyAnimClip_Num];

private:
	ModelRender		m_playerModelRender;		//�v���C���[���f��
	ModelRender		m_tresureModelRender;		//���󃂃f��
	ModelRender		m_enemyModelRender[3];		//�G�l�~�[���f��
	ModelRender		m_volumeLightModelRender;	//�{�����[�����C�g���f��
	SpriteRender	m_filmSpriteRender[4];		//�t�B�����摜
	SoundSource*	m_alert = nullptr;
	Fade*			m_fade = nullptr;
	Vector3			m_tresurePos;				//��̍��W
	Vector3			m_cameraPos;				//�J�������W
	Vector3			m_cameraTarget;				//�J���������_
	bool			m_isWaitFadeOut;			//�t�F�[�h�A�E�g��
	int				m_cameraScene = 0;			//�J�����V�[��
	float			m_timer = 0.0f;				//�^�C�}�[
	float			m_filmTimer = 0.0f;			//�t�B�����p�^�C�}�[
};

