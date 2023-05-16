#pragma once
#include "Wall.h"

class Wall_Decoration :public Wall
{
public:
	Wall_Decoration();
	~Wall_Decoration()override;

	bool Start();
	void Load_Model(int num);
	void Render(RenderContext& rc);
};