#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
#include "Game.h"
#include "Enemy.h"
#include "Treasure.h"
#include "GameCamera.h"
#include "GameUI.h"
#include "Stage/Wall/Wall.h"
namespace
{
	const float		PLAYER_WALKING = 30.0f;								//�������̏�Z��
	const float		PLAYER_RUNING = 60.0f;								//���莞�̏�Z��
	const float		JUMP_VOLUM = 200.0f;								//�W�����v��
	const float		GRAVITY = 400.0f;									//�d��
	const float		SPEED_DOWN = 0.8;									//���x������
	const float		PLAYER_STAMINA = 10.0f;								//�v���C���[�̃X�^�~�i
	const float		STAMINA_HEAL = 2.0f;								//�X�^�~�i�̉�
	const float		STAMINA_STOPING_HEAL = 3.0f;						//�v���C���[���~�܂��Ă���Ƃ��̉񕜗�
	const float		STAMINA_DOWN = 2.0f;								//�X�^�~�i�̌���
	const float		STAMINA_BASE_POSITION = 60.0f;						//�X�^�~�i�x�[�X�摜�̍��W
	const float		STAMINA_GAGE_POSITION = 0.0f;						//�X�^�~�i�Q�[�W�摜�̍��W
	const float		STAMINA_COOL_TIME = 1.0f;							//�X�^�~�i���񕜂���܂ł̎���
	const float 	EFFECTSIZE = 0.5f;
}

Player::Player()
{

}

Player::~Player()
{
	if (m_tireEffect != nullptr)
	{
		m_tireEffect->Stop();
	}
}

bool Player::Start()
{

	
	m_stamina = PLAYER_STAMINA;
	
	m_gamecamera = FindGO<GameCamera>("gameCamera");
	m_playerManagement=FindGO<PlayerManagement>("playerManagement");
	m_collisionObject = NewGO<CollisionObject>(0);
	m_game = FindGO<Game>("game");
	//����̌Ăяo��
	m_treasure = FindGO<Treasure>("treaSure");
	m_gameUI = FindGO<GameUI>("gameUI");
	m_gameUI->StaminaGage(m_stamina,m_position);
	//�N�[���^�C����������
	m_staminaCoolTime = STAMINA_COOL_TIME;
	return true;
}
void Player::Animation3D()
{
	//�A�j���[�V������ǂݍ���
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/player/walk_key.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/player/run_key.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Jump].Load("Assets/animData/player/jump_start.tka");
	m_animationClips[m_enAnimationClip_Jump].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Jumpend].Load("Assets/animData/player/jump_end.tka");
	m_animationClips[m_enAnimationClip_Jumpend].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Down].Load("Assets/animData/player/confusion.tka");
	m_animationClips[m_enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_DownEnd].Load("Assets/animData/player/idle_act.tka");
	m_animationClips[m_enAnimationClip_DownEnd].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Throw].Load("Assets/animData/player/attack.tka");
	m_animationClips[m_enAnimationClip_Throw].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_CaughtStart].Load("Assets/animData/player/sleep_start.tka");
	m_animationClips[m_enAnimationClip_CaughtStart].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Caught].Load("Assets/animData/player/sleep.tka");
	m_animationClips[m_enAnimationClip_Caught].SetLoopFlag(true);
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
	//�s���ł���Ȃ�
	if (m_Player_Act&&
		m_playerManagement->GetGameState()
		) {
		
		Rotation();
		Move();
		ItemChange();
		//2D�Ȃ炨��̋߂��ɋ��Ă��W�����v������
		//�X�^�~�i���Ȃ����̓W�����v�ł��Ȃ��悤�ɂ���
		if (m_playerManagement->m_enMananagementState == m_playerManagement->m_enPlayer_2DChanging&&
			m_runFlag != false)
		{
			Jump();
		}
		else if (m_Player_Act && m_playerManagement->GetGameState())
		{
			//����̃R���W�������Ȃ����͕��ʂɃW�����v������
			if (m_treasure->GetCollision() == nullptr ||
				m_collisionObject->IsHit(m_treasure->GetCollision()) == false&&
				m_runFlag != false)
			{
				Jump();
			}
		}
		//�v���C���[��������āA�W�����v���ĂȂ��đ�����ԂȂ�R�{�^���������ꂽ�Ƃ���
		if (g_pad[0]->IsTrigger(enButtonRB1)&& 
			m_Player_Act&&
			m_playerState!=m_enPlayer_Jump&&
			m_runFlag==true)
		{
			Throw();
		}
		m_Player_Act = true;
	}

	//�󒆂ɂ���ꍇ
	if(m_characon->IsOnGround() == false)
	{
		//�d�͂𔭐�������
		m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	}

	PlayerCatch();
	Animation();
	ManageState();
	if (!m_treasure->GetHitState()&&
		!m_gameUI->GetCircleDrawState())
	{
		m_gameUI->StaminaGage(m_stamina, m_position);
	}
	
	for (int i=0; i < m_game->GetWallList().size(); i++)
	{
		m_game->GetWallList()[i]->SetWallRenderPosition(m_position);
	}

}

void Player::Move()
{
	m_Lstic.x = 0.0f;
	m_Lstic.z = 0.0f;
	//���x��������
	m_moveSpeed.x *= SPEED_DOWN;
	m_moveSpeed.z *= SPEED_DOWN;
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
	if (m_runFlag == true)
	{
		//����A�{�^����������Ă���Ȃ�
		if (g_pad[0]->IsPress(enButtonA) &&
			m_characon->IsOnGround() == true)
		{
			//�v���C���[�������Ă���Ȃ�
			if (m_Lstic.x != 0 || m_Lstic.y != 0)
			{
				PlayerRun();
			}
				
		}
		else
		{
			//�N�[���^�C�����Ȃ��Ȃ�܂ŉ񕜂��Ȃ�
			m_staminaCoolTime -= g_gameTime->GetFrameDeltaTime();
			if (m_staminaCoolTime < 0.0f)
			{
				//�v���C���[�������Ă��Ȃ��Ȃ�
				if (m_moveSpeed.x >= 0.0f &&
					m_moveSpeed.y >= 0.0f &&
					m_moveSpeed.z >= 0.0f)
				{
					//�X�^�~�i�̉񕜗ʂ�1.5�ɂ���
					m_stamina += STAMINA_STOPING_HEAL * g_gameTime->GetFrameDeltaTime();
					m_stamina = min(m_stamina, PLAYER_STAMINA);
				}
				else
				{
					//�X�^�~�i�̉񕜗ʂ�1�ɂ���
					m_stamina += STAMINA_HEAL * g_gameTime->GetFrameDeltaTime();
					m_stamina = min(m_stamina, PLAYER_STAMINA);
				}

			}
			//���X�e�b�N�ƕ������x����Z������
			m_moveSpeed += cameraFoward * m_Lstic.y * PLAYER_WALKING;
			m_moveSpeed += cameraRight * m_Lstic.x * PLAYER_WALKING;
		}
	}
	else
	{
		if (m_stamina == 0.0f)
		{
			TireEffect();
		}
		
		//�X�^�~�i���Ȃ��ꍇ�s���ł��Ȃ��悤�ɂ���
		m_moveSpeed *=0.9;
		//�X�^�~�i�̉񕜂���
		m_stamina += STAMINA_STOPING_HEAL * g_gameTime->GetFrameDeltaTime();
		m_stamina = min(m_stamina, PLAYER_STAMINA);
		
		//�X�^�~�i���S�������Ȃ�
		if (m_stamina == PLAYER_STAMINA)
		{
			//�����悤�ɂ���
			m_runFlag = true;
			m_tireEffect->Stop();
		}
	}
	
	
}

void Player::PlayerRun()
{
	m_stamina -= STAMINA_DOWN * g_gameTime->GetFrameDeltaTime();
	m_stamina = max(m_stamina, 0.0f);
	if (m_stamina <= 0.0f)
	{
		m_runFlag = false;
	}
	//�J�����̑O�����ƁA�E�����̎擾
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������ƉE�������擾
	cameraFoward.y = 0.0f;
	cameraFoward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//�_�b�V����������
	//���X�e�b�N�Ƒ��鑬�x����Z����
	m_moveSpeed += cameraFoward * m_Lstic.y * PLAYER_RUNING;
	m_moveSpeed += cameraRight * m_Lstic.x * PLAYER_RUNING;
	m_staminaCoolTime = STAMINA_COOL_TIME;
}

void Player::TireEffect()
{
	m_tireEffect = NewGO<EffectEmitter>(0);
	m_tireEffect->Init(7);

	Vector3 effectPos = m_position;
	Vector3 move = m_moveSpeed;
	move.y = 0.0f;
	
	Quaternion rot = Quaternion::Identity;
	//���]������
	move *= -1.0f;
	move.Normalize();
	//���]�������x��Y������̉�]�ɓK�p����
	rot.SetRotationYFromDirectionXZ(move);
	//�������ɂ���
	effectPos +=move * 10.0f;
	effectPos.y = 120.0f;

	m_tireEffect->SetPosition(effectPos);
	m_tireEffect->SetScale(Vector3::One * EFFECTSIZE);

	m_tireEffect->SetRotation(rot);
	m_tireEffect->Play();
	m_tireEffect->Update();
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
			m_moveSpeed.y = JUMP_VOLUM;
			m_Player_Act = false;

		}
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
		m_enItemState = m_enItem_SoundBom;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		//�����e�������Ă���
		
		m_enItemState = m_enItem_Flash;
	}
}

void Player::Throw()
{
}

void Player::GhostHit()
{
	float NowTargetDiff = D3D12_FLOAT32_MAX;
	for (const auto& ghostposition : m_playerPushPositions)
	{
		//�ł��߂����W�����߂�
		Vector3 diff = ghostposition - GetPosition();
		float lenght = diff.Length();
		if (NowTargetDiff > lenght)
		{
			NowTargetDiff = lenght;
			//�ł��߂����W��^����
			SetGhostPosition(ghostposition);
		}
	}

	
}

void Player::PlayerCatch()
{
	for (int i = 0; i < m_game->GetEnemyList().size(); i++)
	{
		//�G�l�~�[�̃X�e�[�g���߂܂����Ȃ�
		if (m_game->GetEnemyList()[i]->GetEnemyActionState() == m_game->GetEnemyList()[i]->m_ActionState_CatchPlayer)
		{
			m_playerState = m_enPlayer_Caught;
			//�߂܂������̍��W��Ԃ�
			m_gamecamera->SetCameraPositio(m_position);
		}
	}
}

void Player::ProcessCommonStateTransition()
{

	for (int i = 0; i < m_game->GetEnemyList().size(); i++)
	{
		//�G�l�~�[�̃X�e�[�g���߂܂����Ȃ�
		if (m_game->GetEnemyList()[i]->GetEnemyActionState() == m_game->GetEnemyList()[i]->m_ActionState_CatchPlayer && m_playerCaught)
		{
			//�߂܂����X�e�[�g�ɂ���
			m_playerState = m_enPlayer_Caught;
			//�߂܂�������ɂ���
			m_playerCaught = true;
			m_gamecamera->SetCameraPositio(m_position);
			return;
		}
	}
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
		if (g_pad[0]->IsPress(enButtonA)&&
			m_runFlag == true)
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
}
void Player::ProcessWalkStateTransition()
{
}
void Player::ProcessRunStateTransition()
{
}
void Player::ProcessJumpStateTransition()
{
}
void Player::ProcessJumpendStateTransition()
{
}
void Player::ProcessChangeStateTransition()
{
}
void Player::ProcessCatchingStateTransition()
{

}
void Player::ProcessDownStateTransition()
{
}
void Player::ProcessThrowStateTransition()
{
}
void Player::ProcessCaughtStateTransition()
{
}

void Player::ProcessClearStateTransition()
{
}

void Player::ProcessGameOverStateTransition()
{
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
	case m_enPlayer_Down:
		ProcessDownStateTransition();
		break;
		//�߂܂�����
	case m_enPlayer_Caught:
		//�G�t�F�N�g�����݂��Ȃ��Ȃ�
		if (m_tireEffect == nullptr)
		{
			ProcessCaughtStateTransition();
			break;
		}
		//�G�t�F�N�g������Ă��鎞�͎~�߂�
		if (m_tireEffect->IsPlay() == true)
		{
			m_tireEffect->Stop();
		}
		ProcessCaughtStateTransition();
		break;
		//�߂܂��Ă���Ƃ�
	case m_enPlayer_Catching:
		ProcessCatchingStateTransition();
		break;
	default:
		break;
	}
}

void Player::Animation()
{
}

void Player::Render(RenderContext& rc)
{
	
}

