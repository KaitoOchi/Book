#include "stdafx.h"
#include "Object.h"
#include "PlayerManagement.h"

Object::Object()
{
}

Object::~Object()
{
	DeleteGO(m_collisionObject);
}

bool Object::Start()
{
	m_player = FindGO<PlayerManagement>("playerManagement");
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