#pragma once
#include "Player.h"
class Player3D :public Player
{
public:
	Player3D();
	~Player3D();
	bool Start();
	void Update();
	void Animation();
	void Throw();
	void Render(RenderContext& rc);
	enum EnPlayer3DState {
		m_enPlayer3D_Throw,//������
		m_enPlayer3D_Steal,//����
		m_enPlayer3D_Push,//����
	};


	CharacterController m_3Dcharacon;//�L�����R��
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

};

