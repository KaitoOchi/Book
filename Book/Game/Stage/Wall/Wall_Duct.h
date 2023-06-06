#pragma once
#include "Wall.h"

/// <summary>
/// 脱出口モデルクラス。
/// </summary>
class Wall_Duct : public Wall
{
public:
	Wall_Duct();
	~Wall_Duct()override;
	bool Start();
	void Render(RenderContext& rc);
};

