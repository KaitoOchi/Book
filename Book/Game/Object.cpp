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
	//�����蔻��
	//if (m_collisionObject->IsHit(m_player->GetCharacterController())) {

	//	Hit();
	//}
}

void Object::Hit()
{
}