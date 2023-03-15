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
	if (m_player->GetCharacon() == nullptr)
	{
		return;
	}

	//�����蔻��
	if (m_collisionObject->IsHit(*m_player->GetCharacon())) {

		Hit();
	}
}

void Object::Hit()
{
}