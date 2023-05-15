#pragma once
#include "Wall.h"
class Wall_Gap:public Wall
{
public:
	Wall_Gap();
	~Wall_Gap()override;

	bool Start();
	void ModelLoad(int i);
	void Render(RenderContext& rc);
};

