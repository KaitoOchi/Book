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
	m_boxobje.Create(Vector3(m_boxSize), m_position, m_rotation);
	return true;

}