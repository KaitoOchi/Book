#pragma once
#include "Wall.h"

/// <summary>
/// ���ԃ��f���N���X�B
/// </summary>
class Wall_Gap:public Wall
{
public:
	Wall_Gap();
	~Wall_Gap()override;
	bool Start();
	void Render(RenderContext& rc);
};

