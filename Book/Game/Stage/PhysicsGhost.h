#pragma once
#include "Ghost.h"
class PhysicsGhost:public Ghost
{
public:
	PhysicsGhost();
	~PhysicsGhost()override;
	bool Start();
	PhysicsGhostObject m_physicsGhostObj;
private:
};

