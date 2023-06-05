#pragma once
#include "Wall.h"

/// <summary>
/// 通常壁モデルクラス。
/// </summary>
class Wall_Normal : public Wall
{
public:
	Wall_Normal();
	~Wall_Normal()override;
	bool Start();
	void Render(RenderContext& rc);
};

