#include "stdafx.h"
#include "PlayerManagement.h"

#include "Player2D.h"
#include "Player3D.h"
#include "Enemy.h"
#include "Game.h"
#include "PhysicsGhost.h"
#include "GameCamera.h"
#include "GameManager.h"

namespace
{
	const float CHANGE_TIME = 1.0f;		//変身時間
	const float EFFECTSIZE = 1.5f;		//エフェクトサイズ
}


PlayerManagement::PlayerManagement()
{
	
}
PlayerManagement::~PlayerManagement()
{

}
bool PlayerManagement::Start()
{
	m_physicsghost = FindGO<PhysicsGhost>("physicsGhost");
	m_player2D = FindGO<Player2D>("player2d");
	m_player3D = FindGO<Player3D>("player3d");
	m_gamecamera = FindGO<GameCamera>("gameCamera");
	m_game = FindGO<Game>("game");

	


	return true;
}
void PlayerManagement::Update()
{
	if (!m_GameStartState) {
		return;
	}

	if (m_enMananagementState == m_enPlayer_Stop) {
		return;
	}
	
	for (int i = 0; i < m_game->GetEnemyList().size(); i++)
	{
		if (m_game->GetEnemyList()[i]->GetChachPlayerFlag() == true) 
		{
			if (m_player3D->IsActive() == false)
			{
				PlayerChange3D();
				m_player3D->SetPosition(m_player2D->GetPosition());
			}
			else
			{
				return;
			}
			
		}
		
	}


	if (m_enMananagementState == m_enPlayer_Changing) {
		IsChanging();
		return;
	}
	if (m_player3D->m_Player_Act == true || 
		m_player2D->m_Player_Act == true)
	{
		if (m_player3D->GetPlayerState() != Player::m_enPlayer_Down)
		{
			Input();
		}
		
	}
	
	
}
void PlayerManagement::Input()
{
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		
		switch (m_enMananagementState)
		{
			//2D�̏ꍇ3D��Ăяo��
		case PlayerManagement::m_enPlayer_2DChanging:
			m_manageStateTmp = m_enPlayer_3DChanging;
			m_player2D->m_Player_Act = false;
			m_player2D->SetMoveSpeed(Vector3::Zero);
			SetPosition(m_player2D->GetPosition());
			//�J�����̈ʒu�̐ݒ�
			m_gamecamera->SetCameraPositio(m_player2D->GetPosition());
			
			break;
			//3D�̏ꍇ2D��Ăяo��
		case PlayerManagement::m_enPlayer_3DChanging:
			m_manageStateTmp = m_enPlayer_2DChanging;
			m_player3D->m_Player_Act = false;
			m_player3D->SetMoveSpeed(Vector3::Zero);
			SetPosition(m_player3D->GetPosition());
			//�J�����̈ʒu�̐ݒ�
			m_gamecamera->SetCameraPositio(m_player3D->GetPosition());
			break;
		}

		//�ϐg����o��
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(13);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());

		//煙エフェクトの再生
		EffectEmitter* smokeEffect = NewGO<EffectEmitter>(0);
		smokeEffect->Init(0);
		smokeEffect->SetPosition(m_position);
		smokeEffect->SetScale(Vector3::One * EFFECTSIZE);
		smokeEffect->Play();
		smokeEffect->Update();

		m_enMananagementState = m_enPlayer_Changing;
	}
}

void PlayerManagement::SetChange(EnManagementState manaState)
{
	m_player3D->m_Player_Act = false;
	m_player3D->SetMoveSpeed(Vector3::Zero);
	m_player2D->m_Player_Act = false;
	m_player2D->SetMoveSpeed(Vector3::Zero);
	m_manageStateTmp = manaState;
	m_enMananagementState = m_enPlayer_Changing;
}

void PlayerManagement::PlayerChange2D()
{
	m_player2D->Activate();//�v���C���[2D��A�N�e�B�u�ɂ���
	m_player2D->SetPosition(m_player3D->GetPosition());//2D��3D�̃|�W�V������^����
	m_player2D->SetStamina(m_player3D->GetStamina());
	m_player2D->ModelRenderUpdate();//���f����X�V����
	m_player3D->PlayerChang();//�v���C���[3D��f�B�A�N�e�B�u�ɂ���
	m_player2D->CreatCharcon();//�L�����R���𐶐�����
	SetCharacon(m_player2D->GetCharacon());//�L�����R���̏��𓾂�
	//�v���C���[��2D�ɂ���
	m_enMananagementState = m_enPlayer_2DChanging;
	
}
void PlayerManagement::PlayerChange3D()
{
	m_player3D->Activate();//�v���C���[3D��A�N�e�B�u�ɂ���
	m_player3D->SetPosition(m_player2D->GetPosition());//3D��2D�̃|�W�V������^����
	m_player3D->SetStamina(m_player2D->GetStamina());
	m_player3D->ModelRenderUpdate();//���f����X�V����
	m_player2D->PlayerChang();//�v���C���[2D��f�B�A�N�e�B�u�ɂ���
	m_player3D->CreatCharcon();//�L�����R���𐶐�����
	SetCharacon(m_player3D->GetCharacon());//�L�����R���̏��𓾂�
	//�v���C���[�����܂��Ă���Ȃ�
	PhysicsWorld::GetInstance()->ContactTest(*m_player3D->GetCharacon(), [&](const btCollisionObject& contactObject) {
		for (int i = 0; i < m_game->GetPhysicsGhostList().size(); i++)
		{
			if (m_game->GetPhysicsGhostList()[i]->m_physicsGhostObj.IsSelf(contactObject) == true)
			{
				m_player3D->GhostHit();
				m_player3D->SetPushPosition(m_player3D->GetPosition());
				m_player3D->m_ghostHit = false;
			}
		}

		});
	//�v���C���[��RD�ɂ���
	m_enMananagementState = m_enPlayer_3DChanging;
}

void PlayerManagement::IsChanging()
{
	if (m_changeTime > CHANGE_TIME) {

		//�X�e�[�g��ύX����
		switch (m_manageStateTmp)
		{
			//2D�̏ꍇ3D��Ăяo��
		case PlayerManagement::m_enPlayer_3DChanging:
			PlayerChange3D();
			m_player3D->m_Player_Act = true;
			
			break;
			//3D�̏ꍇ2D��Ăяo��
		case PlayerManagement::m_enPlayer_2DChanging:
			PlayerChange2D();
			m_player2D->m_Player_Act = true;
		
			break;
		default:
			break;
		}

		m_changeTime = 0.0f;
		return;
	}

	m_changeTime += g_gameTime->GetFrameDeltaTime();
}