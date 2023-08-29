#include "stdafx.h"
#include "Player2D.h"
#include "Player3D.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
#include "Enemy.h"
#include "Game.h"
#include "GameUI.h"
namespace
{
	const Vector3	BOXSIZE{ 20.0f,120.0f,2.0f };						//�{�b�N�X�R���C�_�[�̑傫��
	const Vector3	MODELSIZE{ 1.0f,1.0f,1.0f };						//���f���T�C�Y
	const float		SPEED_DOWN = 0.8;									//���x������
	const float		PLAYER_STAMINA = 10.0f;								//�v���C���[�̃X�^�~�i
	const float		EFFECT_SIZE = 1.5f;									//�G�t�F�N�g�T�C�Y
}
Player2D::Player2D()
{

}
Player2D::~Player2D()
{
	if (IsActive())
	{
		delete m_changeAnimation;
		delete(m_characon);
		delete(m_modelRender);
	}
}
bool Player2D::Start()
{
	m_changeAnimation[enAnimationClip_Idle].Load("Assets/animData/player_2D/player2D_idle.tka");
	m_changeAnimation[enAnimationClip_Idle].SetLoopFlag(false);
	m_changeAnimation[enAnimationClip_Change].Load("Assets/animData/player_2D/player2D_change.tka");
	m_changeAnimation[enAnimationClip_Change].SetLoopFlag(false);

	m_characon = new CharacterController;
	m_modelRender = new ModelRender;
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_player3D = FindGO<Player3D>("player3d");
	//���f���̓ǂݍ���
	m_modelRender->Init("Assets/modelData/player/player2D.tkm", m_changeAnimation, enAnimationClip_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_2DPlayer, D3D12_CULL_MODE_NONE);
	m_modelRender->SetScale(MODELSIZE);
	m_modelRender->PlayAnimation(enAnimationClip_Idle, 0.0f);
	m_modelRender->Update();

	
	m_characon->Init(BOXSIZE, m_position);
	delete(m_characon);
	m_characon = nullptr;

	Player::Start();

	Deactivate();

	return true;
}
void Player2D::Update()
{
	Player::Update();
	if (GetCharacon() == nullptr)
	{
		return;
	}

	//�L�����R���𓮂���
	m_characon->SetPosition(m_position);
	//�L�����R������]������
	m_characon->SetRotaition(m_rotation);
	//�L�����R���𗘗p���ăv���C���[�𓮂���
	m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime()/2.0f);

	//atan2���g�p���ĉ�]�p�x�����߂�
	Player::Rotation2D();

	//���f���̍��W�Ɖ�]���X�V����
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->Update();	

	if (!GetRunState())
	{
		m_gameUI->SetChangeGaugeState(false);
	}
	else
	{
		m_gameUI->SetChangeGaugeState(true);
	}
}
void Player2D::PlayerChang()
{
	delete(m_characon);
	m_characon = nullptr;
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
	Deactivate();
}

void Player2D::CreatCharcon()
{
	m_characon = new CharacterController;
	m_characon->Init(BOXSIZE, GetPosition());
}

void Player2D::Animation()
{
	switch (m_playerState)
	{
	case Player::m_enPlayer_Idle:
		m_addValue++;
		m_playAnimationSpeed = m_addValue / 10;
		if (m_addValue >= 29)
		{
			m_addValue = 0;
		}
		break;
	case Player::m_enPlayer_walk:
		//�����A�j���[�V�����̐؂�ւ�
		m_walkAnimationSpeed++;
		m_playAnimationSpeed = m_walkAnimationSpeed / 10;
		if (m_walkAnimationSpeed >= 89)
		{
			m_walkAnimationSpeed = 30;
		}
		break;
	case Player::m_enPlayer_Run:
		//�����A�j���[�V�����𑁂�����
		m_playAnimationSpeed = m_runAnimationSpeed;

		m_runAnimationSpeed += 0.2;
		if (m_runAnimationSpeed >= 8.8)
		{
			m_runAnimationSpeed = 3.0f;
		}
		break;
	case Player::m_enPlayer_Jump:
	case Player::m_enPlayer_Jumpend:
		//�W�����v�A�j���[�V�����؂�ւ�
		m_playAnimationSpeed = m_jumpAnimationSpeed / 10;
		m_jumpAnimationSpeed++;
		if (m_jumpAnimationSpeed >= 130)
		{
			m_jumpAnimationSpeed = 130;
		}
		break;
	case Player::m_enPlayer_Change:
		m_modelRender->PlayAnimation(enAnimationClip_Change, 0.0f);
		break;
	default:
		break;
	}

	RenderingEngine::GetInstance()->GetLightCB().shadowCB.playerAnim2D = m_playAnimationSpeed;
}

void Player2D::ProcessIdleStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}
void Player2D::ProcessWalkStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}
void Player2D::ProcessRunStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}
void Player2D::ProcessJumpStateTransition()
{
	m_playerState = m_enPlayer_Jumpend;
}
void Player2D::ProcessJumpendStateTransition()
{
	if (m_characon->IsOnGround())
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}

}
void Player2D::ProcessChangeStateTransition()
{
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
		m_modelRender->PlayAnimation(enAnimationClip_Idle, 0.0f);
		m_modelRender->Update();
	}
}
void Player2D::ProcessFoundStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player2D::ProcessStealStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player2D::ProcessCaughtStateTransition()
{
	if (!m_chactState2D)
	{
		m_playerManagement->SetChange(m_playerManagement->m_enPlayer_3DChanging);
		m_player3D->SetPlayerState(m_player3D->m_enPlayer_Caught);
		m_playerState = m_enPlayer_Catching;
		m_chactState2D = true;
	}
}

void Player2D::ProcessClearStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player2D::ProcessGameOverStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player2D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}

void Player2D::Throw()
{
}
void Player2D::ProcessDownStateTransition()
{
}
void Player2D::ProcessThrowStateTransition()
{
}
void Player2D::ProcessCatchingStateTransition()
{
}