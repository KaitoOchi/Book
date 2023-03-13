#include "stdafx.h"
#include "Player.h"
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
	return true;
}

void Player::Update()
{
	//�����Ă���Ƃ��ɍs���o���Ȃ��悤�ɂ���
	if (m_playerState!=m_enPlayer3D_Throw) {
		Move();
		Jump();
		Rotation();
	}
	
	
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
	m_Lstic.z = g_pad[0]->GetLStickYF();
	//����A�{�^����������Ă���Ȃ�
	if (g_pad[0]->IsPress(enButtonA))
	{
		//�_�b�V����������
		//���X�e�b�N�Ƒ��鑬�x����Z����
		m_moveSpeed.x += m_Lstic.x * RUN;
		m_moveSpeed.z += m_Lstic.z * RUN;
	}
	else
	{
		//���X�e�b�N�ƕ������x����Z������
		m_moveSpeed.x += m_Lstic.x * WALK;
		m_moveSpeed.z += m_Lstic.z * WALK;
	}

}
void Player::Jump()
{
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
	//atan2���g�p���ĉ�]�p�x�����߂�
	angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//SetRotation���g�p����
	m_rotation.SetRotationY(-angle);
}
void Player::Change(bool m_change)
{
	if (m_change)
	{
		m_playerState = m_enPlayer_3DChanging;

	}
	else
	{
		m_playerState = m_enPlayer_2DChanging;
	}
}	

void Player::ProcessCommonStateTransition()
{
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
void Player::Process2DChangingStateTransition()
{
	
}
void Player::Process3DChangingStateTransition()
{
	
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
	//case m_enPlayer_2DChanging:
	//	//�ؑփX�e�[�g�̃X�e�[�g�J�ڏ����B
	//	Process2DChangingStateTransition();
	//	break;
	//case m_enPlayer_3DChanging:
	//	//�ؑփX�e�[�g�̃X�e�[�g�J�ڏ����B
	//	Process3DChangingStateTransition();
	//	break;
	     //������Ƃ�
	case m_enPlayer3D_Throw:
		ProcessThrowStateTransition();
		break;
	default:
		break;
	}
}