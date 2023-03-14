#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
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

void Object::Hit()
{
}