#include "stdafx.h"
#include "Player3D.h"
#include"Player2D.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
namespace
{
	const Vector3 BOXSIZE{ 50.0f,120.0f,50.0f };//�{�b�N�X�R���C�_�[�̑傫��
}
Player3D::Player3D()
{

}
Player3D::~Player3D()
{
	delete(m_modelRender);
}
bool Player3D::Start()
{
	m_characon = new CharacterController;
	Player::Start();
	m_characon->Init(BOXSIZE, m_position);
	m_modelRender= new ModelRender;
	//�}�l�W�����g�̌Ăяo��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
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
	m_modelRender->Init("Assets/modelData/player/player.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ, true, D3D12_CULL_MODE_NONE);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(Quaternion::Identity);
	m_modelRender->SetScale(Vector3::One);
	m_modelRender->Update();
	return true;
}

void Player3D::Update()
{
	if (GetCharacon() == nullptr)
	{
		return;
	}
	//atn2�łRD�̉�]�����߂�
	angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	Player::Update();
	Animation();
	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		Throw();
	}
	//�v���C���[�̈ړ����p������B
	//�L�����R���ō��W���ړ�������B
	m_characon->SetPosition(m_position);
	m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->Update();
}
void Player3D::Throw()
{
	m_playerState = m_enPlayer3D_Throw;
}
void Player3D::PlayerChang()
{
	delete(m_characon);
	m_characon = nullptr;
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
	Deactivate();
	
}
//�L�����R���̍쐬�֐�
void Player3D::CreatCharcon()
{
	m_characon = new CharacterController;
	m_characon->Init(BOXSIZE, GetPosition());
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