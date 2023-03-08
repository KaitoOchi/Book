#include "stdafx.h"
#include "Player.h"


Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	//�L�����R��������������B
	m_characon.Init(25.0f, 75.0f, m_position);
	return true;
}

void Player::Update()
{
	
	Move();
	Jump();
	Rotation();
	ManageState();
	
}

void Player::Move()
{
	//���X�e�b�N�̓��͗ʂ�������
	m_Lstic.x = 0.0f;
	m_Lstic.z = 0.0f;
	//�ړ����x��������
	m_moveSpeed.x*=0.8f;
	m_moveSpeed.z *= 0.8f;
	//���X�e�b�N�̓��͗ʂ��擾1
	m_Lstic.x = g_pad[0]->GetLStickXF();
	m_Lstic.z = g_pad[0]->GetLStickYF();
	//����B�{�^����������Ă���Ȃ�
	if (g_pad[0]->IsPress(enButtonA))
	{
		//�_�b�V�����ɂ���
		//�ړ����x�ɓ��͗ʂ����Z����
		m_moveSpeed.x += m_Lstic.x * m_run;
		m_moveSpeed.z += m_Lstic.z * m_run;
	}
	else
	{
		//�ړ����x�ɓ��͗ʂ����Z����
		m_moveSpeed.x += m_Lstic.x * m_walk;
		m_moveSpeed.z += m_Lstic.z * m_walk;
	}
	
}
void Player::Jump()
{
	//�n�ʂɕt���Ă�����B
	if (m_characon.IsOnGround()==true)
	{
		//�d�͂𖳂����B
		m_moveSpeed.y = 0.0f;
		//B�{�^���������ꂽ��B
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//�W�����v������B
			m_moveSpeed.y = m_jumpvolume;
			
		}
	}
	//�n�ʂɕt���Ă��Ȃ�������B
	else
	{
		//�d�͂𔭐�������B
		m_moveSpeed.y -= 400.0f * g_gameTime->GetFrameDeltaTime();
	}
}

void Player::Rotation()
{
	//�����Ă��Ȃ����߉�]���Ȃ�
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}
	//atan2�Ŋp�x�����߂āA�������]�p�x�ɂ��Ă���
	angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//���W�A���P�ʂ̂���SetRotaition���g�p����
	m_rotation.SetRotationY(-angle);
}
void Player::ProcessCommonStateTransition()
{
	if (m_characon.IsOnGround() == false)
	{
		//�X�e�[�g���W�����v���ɂ���
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
			//�������ɂ���B
			m_playerState = m_enPlayer_walk;
		}
	}
	else
	{
		//�X�e�[�g��ҋ@���ɂ���
		m_playerState = m_enPlayer_Idle;
	}
	
}
void Player::ProcessIdleStateTransition()
{
	//�X�e�[�g�̑J�ڏ���
	ProcessCommonStateTransition();
}
void Player::ProcessWalkStateTransition()
{
	//�X�e�[�g�̑J�ڏ���
	ProcessCommonStateTransition();
}
void Player::ProcessRunStateTransition()
{
	//�X�e�[�g�̑J�ڏ���
	ProcessCommonStateTransition();
}
void Player::ProcessJumpStateTransition()
{
	//�X�e�[�g�̑J�ڏ���
	ProcessCommonStateTransition();
}
void Player::ProcessJumpingStateTransition()
{
	//�X�e�[�g�̑J�ڏ���
	ProcessCommonStateTransition();
}
void Player::ProcessChangeStateTransition()
{
	//�X�e�[�g�̑J�ڏ���
	ProcessCommonStateTransition();
}
void Player::ManageState()
{
	switch (m_playerState)
	{
	//�ҋ@�X�e�[�g�̎�
	case m_enPlayer_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessIdleStateTransition();
		break;
	//�����X�e�[�g�̎�
	case m_enPlayer_walk:
		//�����X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessWalkStateTransition();
		break;
	//����X�e�[�g�̎�
	case m_enPlayer_Run:
		//����X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessRunStateTransition();
		break;
	//�W�����v�X�e�[�g�̎�
	case m_enPlayer_Jump:
		//�W�����v�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessJumpStateTransition();
		break;
		//�W�����v���X�e�[�g�̎�
	case m_enPlayer_Jumping:
		//�W�����v�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessJumpingStateTransition();
		break;
	//�ؑփX�e�[�g�̎�
	case m_enPlayer_Change:
		//�ؑփX�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessChangeStateTransition();
		break;
	default:
		break;
	}
}