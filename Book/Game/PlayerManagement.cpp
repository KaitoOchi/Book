#include "stdafx.h"
#include "Player2D.h"
#include"Player3D.h"
#include "PlayerManagement.h"
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
	return true;
}
void PlayerManagement::Update()
{
	PlayerChange();
}
void PlayerManagement::ProcessCommonStateTransition()
{

}
void PlayerManagement::PlayerChange()
{
	if (g_pad[0]->IsTrigger(enButtonLB1))
	{
		switch (m_enMnanagementState)
		{
		case PlayerManagement::m_enPlayer_2DChanging:
			m_player3D->Activate();//プレイヤー3Dをアクティブにする
			m_player3D->SetPosition(m_player2D->GetPosition());//3Dに2Dのポジションを与える
			m_player3D->ModelRenderUpdate();//モデルを更新する
			m_player2D->PlayerChang();//プレイヤー2Dをディアクティブにする
			m_player3D->CreatCharcon();//キャラコンを生成する
			SetCharacon(m_player3D->GetCharacon());//キャラコンの情報を得る
			//プレイヤーを３Dにする
			m_enMnanagementState = m_enPlayer_3DChanging;
			break;
		case PlayerManagement::m_enPlayer_3DChanging:
			m_player2D->Activate();//プレイヤー2Dをアクティブにする
			m_player2D->SetPosition(m_player3D->GetPosition());//2Dに3Dのポジションを与える
			m_player2D->ModelRenderUpdate();//モデルを更新する
			m_player3D->PlayerChang();//プレイヤー3Dをディアクティブにする
			m_player2D->CreatCharcon();//キャラコンを生成する
			SetCharacon(m_player2D->GetCharacon());//キャラコンの情報を得る
			m_enMnanagementState = m_enPlayer_2DChanging;
			break;
		default:
			break;
		}
	}
}