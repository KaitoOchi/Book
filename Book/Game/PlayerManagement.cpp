#include "stdafx.h"
#include "Player2D.h"
#include"Player3D.h"
#include "PlayerManagement.h"
#include "TransparentBox.h"
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
	m_trans = FindGO<TransparentBox>("transparentBox");
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
			//2Dの場合3Dを呼び出す
		case PlayerManagement::m_enPlayer_2DChanging:
			PlayerChange3D();
			break;
			//3Dの場合2Dを呼び出す
		case PlayerManagement::m_enPlayer_3DChanging:
			PlayerChange2D();
			break;
		default:
			break;
		}
	}
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
	m_enMnanagementState = m_enPlayer_2DChanging;
}
void PlayerManagement::PlayerChange3D()
{
	m_player3D->Activate();//プレイヤー3Dをアクティブにする
	m_player3D->SetPosition(m_player2D->GetPosition());//3Dに2Dのポジションを与える
	m_player3D->ModelRenderUpdate();//モデルを更新する
	m_player2D->PlayerChang();//プレイヤー2Dをディアクティブにする
	m_player3D->CreatCharcon();//キャラコンを生成する
	SetCharacon(m_player3D->GetCharacon());//キャラコンの情報を得る
	
	//プレイヤーを３Dにする
	m_enMnanagementState = m_enPlayer_3DChanging;
}