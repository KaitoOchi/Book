#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
	DeleteGO(m_collisionObject);
}

bool Object::Start()
{
	return true;
}

void Object::Update()
{
	Collision();
}

void Object::Collision()
{
	//“–‚½‚è”»’è
	//if (m_collisionObject->IsHit(m_player->GetCharacterController())) {

	//	Hit();
	//}
}

void Object::Hit()
{
}