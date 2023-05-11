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
	/// �A�j���[�V���������B
	/// </summary>
	void Animation();

	/// <summary>
	/// �J�����̏����B
	/// </summary>
	void Camera();

private:
	enum EnAnimationClip {
		animationClip_Idle,
		animationClip_Put,
		animationClip_Num,
	};
	AnimationClip m_animationClips[animationClip_Num];

	enum EnEnemyAnimClip {
		enemyAnimClip_Run,
		enemyAnimClip_Num,
	};
	AnimationClip m_enemyAnimClips[enemyAnimClip_Num];

private:
	ModelRender		m_playerModelRender;
	ModelRender		m_enemyModelRender[3];
	ModelRender		m_volumeLightModelRender;
	Fade*			m_fade = nullptr;
	Vector3			m_tresurePos;			//��̍��W
	Vector3			m_cameraPos;			//�J�������W
	Vector3			m_cameraTarget;			//�J���������_
	bool			m_isWaitFadeOut;		//�t�F�[�h�A�E�g��
	int				m_cameraScene = 0;		//�J�����V�[��
	float			m_timer = 0.0f;			//�^�C�}�[
};

