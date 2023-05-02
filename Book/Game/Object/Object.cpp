#include "stdafx.h"
#include "Object.h"
#include "PlayerManagement.h"
#include "Player3D.h"
#include "Game.h"
Object::Object()
{
}

Object::~Object()
{
	DeleteGO(m_collisionObject);
}

bool Object::Start()
{
	m_player3d = FindGO<Player3D>("player3d");
	m_player = FindGO<PlayerManagement>("playerManagement");
	m_game = FindGO<Game>("game");
	return true;
}

void Object::Update()
{
	Collision();
}

void Object::Collision()
{
	if (m_player->GetCharacon() == nullptr)
	{
		return;
	}

	//�����蔻��
	switch (m_player->m_enMananagementState)
	{
	case PlayerManagement::m_enPlayer_3DChanging:
		if (m_collisionObject->IsHit(*m_player->GetPlayer3D()->GetCharacon())) {

			Hit();
		}
		break;
	case PlayerManagement::m_enPlayer_2DChanging:
		if (m_collisionObject->IsHit(*m_player->GetPlayer2D()->GetCharacon())) {

			Hit();
		}
		break;
	default:
		break;
	}
	
}