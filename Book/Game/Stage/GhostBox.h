#pragma once
#include "Ghost.h"

class GhostBox :public Ghost
{
public:
	GhostBox();
	~GhostBox()override;
	bool Start();

private:
	PhysicsBoxObject m_boxobje;
};

