#pragma once
#include "Wall.h"

/// <summary>
/// �E�o�����f���N���X�B
/// </summary>
class Wall_Duct : public Wall
{
public:
	Wall_Duct();
	~Wall_Duct()override;
	bool Start();
	void Render(RenderContext& rc);
};

