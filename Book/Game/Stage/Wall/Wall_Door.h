#pragma once
#include "Wall.h"
class Wall_Door:public Wall
{
public:
	Wall_Door();
	~Wall_Door()override;

	bool Start();
	void Render(RenderContext& rc);
};

