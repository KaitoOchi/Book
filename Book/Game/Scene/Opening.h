#pragma once

class Fade;

class Opening : public IGameObject
{
public:
	Opening();
	~Opening();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// �v�������B
	/// </summary>
	void Time();

	/// <summary>
	/// ���͏����B
	/// </summary>
	void Input();

	/// <summary>
	/// �J���������B
	/// </summary>
	void Camera();

private:
	enum EnAnimationClip {
		animationClip_Idle,
		animationClip_Walk,
		animationClip_Put,
		animationClip_Num,
	};
	AnimationClip m_animationClips[animationClip_Num];

private:
	ModelRender						m_playerModelRender;			//�v���C���[���f��

	std::array< SpriteRender, 2 >	m_skipSpriteRender;				//�X�L�b�v�摜
	std::array< SpriteRender, 4 >	m_filmSpriteRender;				//�t�B�����摜

	Fade*							m_fade = nullptr;

	Vector3							m_cameraPos;					//�J�������W
	Vector3							m_cameraTarget;					//�J���������_
		
	bool							m_isWaitFadeOut = false;		//�t�F�[�h�A�E�g���ǂ���
	int								m_cameraScene = 0;				//�J�����V�[��
	float							m_timer = 0.0f;					//�^�C�}�[
	float							m_filmTimer = 0.0f;
	float							m_degree = 0.0f;				//�Q�[�W�̊p�x
};