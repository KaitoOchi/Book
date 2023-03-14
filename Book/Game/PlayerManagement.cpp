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
			//�v���C���[3D��2D�̍��W��^����
			m_player3D->SetPosition(m_player2D->GetPosition());
			m_player3D->Activate();
			m_player2D->PlayerChang();
			m_player3D->CreatCharcon();
			//�v���C���[���RD�ɂ���
			m_enMnanagementState = m_enPlayer_3DChanging;
			break;
		case PlayerManagement::m_enPlayer_3DChanging:
			//�v���C���[2D��3D�̍��W��^����
			m_player2D->SetPosition(m_player3D->GetPosition());
			m_player2D->Activate();
			m_player3D->PlayerChang();
			m_player2D->CreatCharcon();
			m_enMnanagementState = m_enPlayer_2DChanging;
			break;
		default:
			break;
		}
	}
}