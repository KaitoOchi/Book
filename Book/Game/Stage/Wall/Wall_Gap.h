#pragma once
#include "Wall.h"
class Wall_Gap:public Wall
{
public:
	Wall_Gap();
	~Wall_Gap()override;

	bool Start();
	void Render(RenderContext& rc);


public:
private:
	
};

