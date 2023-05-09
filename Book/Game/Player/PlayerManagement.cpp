#include "stdafx.h"
#include "Player2D.h"
#include "Game.h"
#include "Player3D.h"
#include "PlayerManagement.h"
#include "PhysicsGhost.h"
#include"GameCamera.h"
namespace
{
	const float CHANGE_TIME = 1.0f;
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
	

	if (m_enMananagementState == m_enPlayer_Changing) {
		IsChanging();
		return;
	}

	Input();
	
	
}
void PlayerManagement::Input()
{
	if (g_pad[0]->IsTrigger(enButtonLB1)) {

		switch (m_enMananagementState)
		{
			//2Dの場合3Dを呼び出す
		case PlayerManagement::m_enPlayer_2DChanging:
			m_manageStateTmp = m_enPlayer_3DChanging;
			m_player2D->m_Player_Act = false;
			m_player2D->SetMoveSpeed(Vector3::Zero);
			m_gamecamera->SetCameraPositio(m_player2D->GetPosition());
			
			break;
			//3Dの場合2Dを呼び出す
		case PlayerManagement::m_enPlayer_3DChanging:
			m_manageStateTmp = m_enPlayer_2DChanging;
			m_player3D->m_Player_Act = false;
			m_player3D->SetMoveSpeed(Vector3::Zero);
			m_gamecamera->SetCameraPositio(m_player3D->GetPosition());
			break;
		}

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
	m_player2D->Activate();//プレイヤー2Dをアクティブにする
	m_player2D->SetPosition(m_player3D->GetPosition());//2Dに3Dのポジションを与える
	m_player2D->ModelRenderUpdate();//モデルを更新する
	m_player3D->PlayerChang();//プレイヤー3Dをディアクティブにする
	m_player2D->CreatCharcon();//キャラコンを生成する
	SetCharacon(m_player2D->GetCharacon());//キャラコンの情報を得る
	//プレイヤーを2Dにする
	m_enMananagementState = m_enPlayer_2DChanging;
}
void PlayerManagement::PlayerChange3D()
{
	m_player3D->Activate();//プレイヤー3Dをアクティブにする
	m_player3D->SetPosition(m_player2D->GetPosition());//3Dに2Dのポジションを与える
	m_player3D->ModelRenderUpdate();//モデルを更新する
	m_player2D->PlayerChang();//プレイヤー2Dをディアクティブにする
	m_player3D->CreatCharcon();//キャラコンを生成する
	SetCharacon(m_player3D->GetCharacon());//キャラコンの情報を得る
	//プレイヤーが埋まっているなら
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
	//プレイヤーを３Dにする
	m_enMananagementState = m_enPlayer_3DChanging;
}

void PlayerManagement::IsChanging()
{
	if (m_changeTime > CHANGE_TIME) {

		//ステートを変更する
		switch (m_manageStateTmp)
		{
			//2Dの場合3Dを呼び出す
		case PlayerManagement::m_enPlayer_3DChanging:
			PlayerChange3D();
			m_player3D->m_Player_Act = true;
			break;
			//3Dの場合2Dを呼び出す
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