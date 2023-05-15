#pragma once
#include "Wall.h"

class Wall_Normal:public Wall
{
public:
	Wall_Normal();
	~Wall_Normal()override;

	bool Start();
	void Render(RenderContext& rc);
};

