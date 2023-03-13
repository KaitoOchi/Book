#pragma once
#include "Player.h"
class Player2D;
class Player3D :public Player
{
public:
	Player3D();
	~Player3D();
	bool Start();
	void Update();
	void Animation();
	void Throw();
	void PlayerChang();
	void Render(RenderContext& rc);
	
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
	Player2D* m_player2D=nullptr;

};

