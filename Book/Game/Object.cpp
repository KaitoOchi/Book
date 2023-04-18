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

	//“–‚½‚è”»’è
	if (m_collisionObject->IsHit(*m_player->GetCharacon())) {

		Hit();
	}
}