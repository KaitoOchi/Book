#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
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
	     //������Ƃ�
	case m_enPlayer3D_Throw:
		ProcessThrowStateTransition();
		break;
	default:
		break;
	}
}
void Player::Animation()
{
}