#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
#include "Ghost.h"
namespace
{
	const float WALK = 40.0f;//�������̏�Z��
	const float RUN = 80.0f;//���莞�̏�Z��
	const float JUMPVOLUM = 200.0f;//�W�����v��
	const float GRAVITY = 400.0f;//�d��
	const float SPEEDDOWN = 0.8;//���x������
}

Player::Player()
{

}

Player::~Player()
{
}

bool Player::Start()
{
	m_playerManagement=FindGO<PlayerManagement>("playerManagement");
	m_collisionObject = NewGO<CollisionObject>(0);
	m_ghost = FindGO<Ghost>("ghost");
	return true;
}
void Player::Animation3D()
{
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
	
}
void Player::Animation2D()
{
	m_player2D[0].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_1.DDS");
	m_player2D[1].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_2.DDS");
	m_player2D[2].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_3.DDS");
	m_player2D[3].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_1.DDS");
	m_player2D[4].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_2.DDS");
	m_player2D[5].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_3.DDS");
	m_player2D[6].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_4.DDS");
	m_player2D[7].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_5.DDS");
	m_player2D[8].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_6.DDS");
	m_player2D[9].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_1.DDS");
	m_player2D[10].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_2.DDS");
	m_player2D[11].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_3.DDS");
	m_player2D[12].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_4.DDS");
	m_player2D[13].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_5.DDS");
}
void Player::Update()
{
	//�����Ă���Ƃ��ɍs���o���Ȃ��悤�ɂ���
	if (m_playerState!=m_enPlayer3D_Throw) {
		Move();
		Jump();
		Rotation();
		ItemChange();
	}
	
	Animation();
	ManageState();

}

void Player::Move()
{
	m_Lstic.x = 0.0f;
	m_Lstic.z = 0.0f;
	//���x��������
	m_moveSpeed.x *= SPEEDDOWN;
	m_moveSpeed.z *= SPEEDDOWN;
	//���X�e�b�N�̏����擾
	m_Lstic.x = g_pad[0]->GetLStickXF();
	m_Lstic.y = g_pad[0]->GetLStickYF();
	//�J�����̑O�����ƁA�E�����̎擾
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������ƉE�������擾
	cameraFoward.y = 0.0f;
	cameraFoward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//����A�{�^����������Ă���Ȃ�
	if (g_pad[0]->IsPress(enButtonA))
	{
		//�_�b�V����������
		//���X�e�b�N�Ƒ��鑬�x����Z����
		m_moveSpeed+= cameraFoward*m_Lstic.y * RUN;
		m_moveSpeed+= cameraRight*m_Lstic.x * RUN;
	}
	else
	{
		//���X�e�b�N�ƕ������x����Z������
		m_moveSpeed += cameraFoward* m_Lstic.y * WALK;
		m_moveSpeed += cameraRight*m_Lstic.x * WALK;
	}

}
void Player::Jump()
{
	if (GetCharacon() == nullptr)
	{
		return;
	}
	//�����n�ʂɂ��Ă���Ȃ�
	if (m_characon->IsOnGround() == true)
	{
		//�d�͂𖳂���
		m_moveSpeed.y = 0.0f;
		//����B�{�^���������ꂽ��
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//�W�����v������
			m_moveSpeed.y = JUMPVOLUM;

		}
	}
	//�󒆂ɂ���ꍇ
	else
	{
		//�d�͂𔭐�������
		m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	}
}
void Player::Rotation()
{
	//���������������Ă��Ȃ��Ȃ�
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}

	
	//SetRotation���g�p����
	m_rotation.SetRotationY(-angle);
}

void Player::ItemChange()
{
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		//�M���e�������Ă���
		m_enItemState = m_enItem_Flash;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		//�����e�������Ă���
		m_enItemState = m_enItem_SoundBom;
	}
}

void Player::GhostHit()
{
	float NowTargetDiff = D3D12_FLOAT32_MAX;
	for (const auto& ghostposition : m_ghostpositions)
	{
		Vector3 diff = ghostposition - GetPosition();
		float lenght = diff.Length();
		if (NowTargetDiff > lenght)
		{
			NowTargetDiff = lenght;
			SetGhostPosition(ghostposition);
		}
	}

	
}
void Player::ProcessCommonStateTransition()
{
	if (GetCharacon() == nullptr)
	{
		return;
	}
	if (m_characon->IsOnGround() == false)
	{
		//�W�����v���ɂ���
		m_playerState = m_enPlayer_Jump;
	}
	else if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		if (g_pad[0]->IsPress(enButtonA))
		{
			//�_�b�V�����ɂ���
			m_playerState = m_enPlayer_Run;
		}
		else
		{
			//�������ɂ���
			m_playerState = m_enPlayer_walk;
		}
	}
	else
	{
		//�ҋ@���ɂ���
		m_playerState = m_enPlayer_Idle;
	}

}
void Player::ProcessIdleStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}
void Player::ProcessWalkStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}
void Player::ProcessRunStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}
void Player::ProcessJumpStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}
void Player::ProcessJumpendStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}
void Player::ProcessChangeStateTransition()
{

	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}
void Player::ProcessDownStateTransition()
{
	//�X�e�[�g�̑J��
	ProcessCommonStateTransition();
}
void Player::ProcessThrowStateTransition()
{
	//���x��������
	m_moveSpeed.x *= SPEEDDOWN;
	m_moveSpeed.z *= SPEEDDOWN;
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}
void Player::ManageState()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎�
	case m_enPlayer_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�����X�e�[�g�̎�
	case m_enPlayer_walk:
		//�����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessWalkStateTransition();
		break;
		//�_�b�V���X�e�[�g�̎�
	case m_enPlayer_Run:
		//�_�b�V���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessRunStateTransition();
		break;
		//�W�����v�X�e�[�g�̎�
	case m_enPlayer_Jump:
		//�W�����v�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessJumpStateTransition();
		break;
		//�W�����v�I���X�e�[�g�̎�
	case m_enPlayer_Jumpend:
		//�W�����v�I���̃X�e�[�g�J�ڏ����B
		ProcessJumpendStateTransition();
		break;
		//�ؑփX�e�[�g�̎�
	case m_enPlayer_Change:
		//�ؑփX�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessChangeStateTransition();
		break;
	     //������Ƃ�
	case m_enPlayer3D_Throw:
		ProcessThrowStateTransition();
		break;
		//�C�₵�Ă���Ƃ�
	case m_enPlayer_Downh:
		ProcessDownStateTransition();
	default:
		break;
	}
}


void Player::Animation()
{
}