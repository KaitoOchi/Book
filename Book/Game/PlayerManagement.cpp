#include "stdafx.h"
#include "Player2D.h"
#include"Player3D.h"
#include "PlayerManagement.h"
#include "Ghost.h"
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
	m_ghost = FindGO<Ghost>("ghost");
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
			//2D�̏ꍇ3D���Ăяo��
		case PlayerManagement::m_enPlayer_2DChanging:
			PlayerChange3D();
			break;
			//3D�̏ꍇ2D���Ăяo��
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
	m_player2D->Activate();//�v���C���[2D���A�N�e�B�u�ɂ���
	m_player2D->SetPosition(m_player3D->GetPosition());//2D��3D�̃|�W�V������^����
	m_player2D->ModelRenderUpdate();//���f�����X�V����
	m_player3D->PlayerChang();//�v���C���[3D���f�B�A�N�e�B�u�ɂ���
	m_player2D->CreatCharcon();//�L�����R���𐶐�����
	SetCharacon(m_player2D->GetCharacon());//�L�����R���̏��𓾂�
	//�v���C���[��2D�ɂ���
	m_enMnanagementState = m_enPlayer_2DChanging;
}
void PlayerManagement::PlayerChange3D()
{
	m_player3D->Activate();//�v���C���[3D���A�N�e�B�u�ɂ���
	m_player3D->SetPosition(m_player2D->GetPosition());//3D��2D�̃|�W�V������^����
	m_player3D->ModelRenderUpdate();//���f�����X�V����
	m_player2D->PlayerChang();//�v���C���[2D���f�B�A�N�e�B�u�ɂ���
	m_player3D->CreatCharcon();//�L�����R���𐶐�����
	SetCharacon(m_player3D->GetCharacon());//�L�����R���̏��𓾂�
	//�v���C���[�����܂��Ă���Ȃ�
	PhysicsWorld::GetInstance()->ContactTest(*m_player3D->GetCharacon(), [&](const btCollisionObject& contactObject) {
			if (m_ghost->m_physicsGhostObj.IsSelf(contactObject) == true)
			{
				GhostHit();
			}
		});
	//�v���C���[���RD�ɂ���
	m_enMnanagementState = m_enPlayer_3DChanging;
}

void PlayerManagement::GhostHit()
{
	float NowTargetDiff = D3D12_FLOAT32_MAX;
	for (const auto& ghostposition :m_ghostpositions)
	{
		Vector3 diff = ghostposition-GetPosition();
		float lenght=diff.Length();
		if (NowTargetDiff > lenght)
		{
			NowTargetDiff = lenght;
			m_ghostPosition = ghostposition;
			m_keepGhostPosition = ghostposition;
		}
	}

	m_player3D->SetPosition(m_ghostPosition);
	
}