#pragma once
#include "Wall.h"

/// <summary>
/// 柱モデルクラス。
/// </summary>
class Wall_Post:public Wall
{
public:
	Wall_Post();
	~Wall_Post()override;

	bool Start();
	void Render(RenderContext& rc);
};

