#pragma once
#include "Player.h"
class PlayerManagement;
class Player3D :public Player
{
public:
	Player3D();
	~Player3D()override;
	bool Start()override;
	void Update()override;
	void Animation()override;
	void Throw();
	void PlayerChang();
	void Render(RenderContext& rc);
	/// <summary>
	/// �L�����R���̍쐬�̊֐�
	/// </summary>
	void CreatCharcon();
private:
	ModelRender m_model3DRender;//3D���f��
	//�A�j���[�V����
	enum EnAnimationClip {
		m_enAnimationClip_Idle,//�ҋ@�A�j���[�V����
		m_enAnimationClip_Walk,//�����A�j���[�V����
		m_enAnimationClip_Run,//����A�j���[�V����
		m_enAnimationClip_Jump,//�W�����v�A�j���[�V����
		m_enAnimationClip_Jumpend,//�W�����v�I���A�j���[�V����
		m_enAnimationClip_Down,//�_�E���A�j���[�V����
		m_enAnimationClip_Throw,//������A�j���[�V����
		m_enAnimationClip_Num,//�A�j���[�V�����̐�
	};
	AnimationClip m_animationClips[m_enAnimationClip_Num];
	PlayerManagement* m_playerManagement = nullptr;

};

