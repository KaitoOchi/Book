#include "stdafx.h"
#include "GhostBox.h"

GhostBox::GhostBox()
{
	
}

GhostBox::~GhostBox()
{
}

bool GhostBox::Start()
{
	Ghost::Start();
	Vector3 boxSize = CreateGhostBox();

	m_boxobje.Create(boxSize, Vector3(m_position.x,m_position.y+50.0f,m_position.z), m_rotation);
	m_boxobje.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);
	
	return true;
}