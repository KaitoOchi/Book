#include "stdafx.h"
#include "Player3D.h"
Player3D::Player3D()
{

}
Player3D::~Player3D()
{

}
bool Player3D::Start()
{
	m_modelRender = new ModelRender;
	Player::Start();
	//�A�j���[�V������ǂݍ���
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/player/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/player/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Jump].Load("Assets/animData/player/jump_start.tka");
	m_animationClips[m_enAnimationClip_Jump].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Jumpend].Load("Assets/animData/player/jump_end.tka");
	m_animationClips[m_enAnimationClip_Jumpend].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Down].Load("Assets/animData/player/die.tka");
	m_animationClips[m_enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Throw].Load("Assets/animData/player/use2.tka");
	m_animationClips[m_enAnimationClip_Throw].SetLoopFlag(false);
	//���f����ǂݍ���
	m_modelRender->Init("Assets/modelData/player/player.tkm",m_animationClips,m_enAnimationClip_Num);
	//�L�����R��������������B
	/*m_3Dcharacon.Init(25.0f, 75.0f, m_position);*/
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(Quaternion::Identity);
	m_modelRender->SetScale(Vector3::One);
	m_modelRender->Update();
	return true;
}

void Player3D::Update() 
{
	Player::Update();
	Animation();
	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		Throw();
	}
	//�v���C���[�̈ړ����p������B
	//�L�����R���ō��W���ړ�������B
	m_position = m_characon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->Update();

}
void Player3D::Throw()
{
	m_playerState = m_enPlayer3D_Throw;
}

void Player3D::Animation()
{
	//�v���C���[�̃X�e�[�g�ɂ���ĕς���
	switch (m_playerState)
	{
	case Player::m_enPlayer_Idle:
		m_modelRender->PlayAnimation(m_enAnimationClip_Idle, 0.5f);
		break;
	case Player::m_enPlayer_walk:
		m_modelRender->PlayAnimation(m_enAnimationClip_Walk, 0.1f);
		break;
	case Player::m_enPlayer_Run:
		m_modelRender->PlayAnimation(m_enAnimationClip_Run, 0.1f);
		break;
	case Player::m_enPlayer_Jump:
		m_modelRender->PlayAnimation(m_enAnimationClip_Jump, 0.8f);
		break;
	case Player::m_enPlayer_Jumpend:
		break;
	case Player::m_enPlayer_Change:
		break;
	case Player::m_enPlayer3D_Throw:
		m_modelRender->PlayAnimation(m_enAnimationClip_Throw, 0.5f);
		break;
	default:
		break;
	}
}
void Player3D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}