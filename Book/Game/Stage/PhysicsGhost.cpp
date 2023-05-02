#include "stdafx.h"
#include "PhysicsGhost.h"
PhysicsGhost::PhysicsGhost()
{

}
PhysicsGhost::~PhysicsGhost()
{

}

bool PhysicsGhost::Start()
{
	Ghost::Start();
	m_physicsGhostObj.CreateBox(
		m_position,
		m_rotation,
		m_boxSize
	);
	m_physicsGhostObj.GetbtCollisionObject().setUserIndex(enCollisionAttr_Ground);
	return true;
}