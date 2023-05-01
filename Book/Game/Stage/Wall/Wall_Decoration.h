#pragma once
#include "Wall.h"

class Wall_Decoration :public Wall
{
public:
	Wall_Decoration();
	~Wall_Decoration();

	bool Start();
	void Render(RenderContext& rc);
};