#include "stdafx.h"
#include "PlayerManagement.h"

#include "Player2D.h"
#include "Player3D.h"
#include "Enemy.h"
#include "Game.h"
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
	m_player2D = FindGO<Player2D>("player2d");
	m_player3D = FindGO<Player3D>("player3d");
	m_gamecamera = FindGO<GameCamera>("gameCamera");
	m_game = FindGO<Game>("game");

	return true;
}
void PlayerManagement::Update()
{
	if (!m_GameStartState && 
		m_enMananagementState == m_enPlayer_Stop
		) {
		return;
	}

	for (int i = 0; i < m_game->GetEnemyList().size(); i++)
	{
		if (m_game->GetEnemyList()[i]->GetChachPlayerFlag() == true) 
		{
			if (m_player3D->IsActive() == false)
			{

				m_smokeEffect->Stop();
				PlayerChange3D();
				m_player3D->SetPosition(m_player2D->GetPosition());
				return;

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
	for (int i = 0; i < m_game->GetEnemyList().size(); i++)
	{
		if (m_player3D->m_Player_Act == true ||
			m_player2D->m_Player_Act == true)
		{
			if (m_player3D->GetPlayerState() != Player::m_enPlayer_Down&&
				m_game->GetEnemyList()[i]->GetChachPlayerFlag() == false)
			{

				Input();
			}

		}
	}	
}
void PlayerManagement::Input()
{
	if (g_pad[0]->IsTrigger(enButtonLB1)) {

		//煙エフェクトを再生
		m_smokeEffect = NewGO<EffectEmitter>(0);
		m_smokeEffect->Init(0);
		m_smokeEffect->SetScale(Vector3::One * EFFECTSIZE);
		m_smokeEffect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);

		switch (m_enMananagementState)
		{
			//2D�̏ꍇ3D��Ăяo��
		case PlayerManagement::m_enPlayer_2DChanging:
			m_manageStateTmp = m_enPlayer_3DChanging;
			m_player2D->m_Player_Act = false;
			m_player2D->SetMoveSpeed(Vector3::Zero);
			//�J�����̈ʒu�̐ݒ�
			m_gamecamera->SetCameraPositio(m_player2D->GetPosition());
			//エフェクトの座標の設定
			m_smokeEffect->SetPosition(m_player2D->GetPosition());
			m_smokeEffect->Play();
			m_smokeEffect->Update();
			break;
			//3D�̏ꍇ2D��Ăяo��
		case PlayerManagement::m_enPlayer_3DChanging:
			m_manageStateTmp = m_enPlayer_2DChanging;
			m_player3D->m_Player_Act = false;
			m_player3D->SetMoveSpeed(Vector3::Zero);
			//�J�����̈ʒu�̐ݒ�
			m_gamecamera->SetCameraPositio(m_player3D->GetPosition());
			//エフェクトの座標の設定
			m_smokeEffect->SetPosition(m_player3D->GetPosition());
			m_smokeEffect->Play();
			m_smokeEffect->Update();
			break;
		}

		//�ϐg����o��
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(13);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());

		m_enMananagementState = m_enPlayer_Changing;
		
	}
}

void PlayerManagement::SetChange(EnManagementState manaState)
{
	
	m_smokeEffect = NewGO<EffectEmitter>(0);
	m_smokeEffect->Init(0);
	//エフェクトの大きさを指定する
	m_smokeEffect->SetScale(Vector3::One * EFFECTSIZE);
	//エフェクトの座標の設定
	m_smokeEffect->Play();
	m_smokeEffect->SetPosition(m_player2D->GetPosition());
	m_smokeEffect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
	m_smokeEffect->Update();


	m_player3D->m_Player_Act = false;
	m_player3D->SetMoveSpeed(Vector3::Zero);
	m_player2D->m_Player_Act = false;
	m_player2D->SetMoveSpeed(Vector3::Zero);
	m_manageStateTmp = manaState;
	m_enMananagementState = m_enPlayer_Changing;
	//スタミナの情報を渡す
	m_player3D->SetStamina(m_player2D->GetStamina());
	m_player3D->SetRunState(m_player2D->GetRunState());

	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(13);
	se->Play(false);
	se->SetVolume(GameManager::GetInstance()->GetSFX());
}


void PlayerManagement::PlayerChange2D()
{
	m_player2D->Activate();//アクティブにする
	m_player2D->SetPosition(m_player3D->GetPosition());
	m_player2D->SetStamina(m_player3D->GetStamina());
	m_player2D->SetRunState(m_player3D->GetRunState());
	m_player2D->SetTireEffect(m_player3D->GetTireEffect());
	m_player2D->SetPlayerState(Player::m_enPlayer_Change);
	m_player2D->ModelRenderUpdate();//モデルの更新
	m_player3D->PlayerChang();//3Dプレイヤーをディアクティブにする
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
	m_player3D->SetRunState(m_player2D->GetRunState());
	m_player3D->SetTireEffect(m_player2D->GetTireEffect());
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