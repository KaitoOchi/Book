#pragma once
#include "Wall.h"

/// <summary>
/// �ʏ�ǃ��f���N���X�B
/// </summary>
class Wall_Normal : public Wall
{
public:
	Wall_Normal();
	~Wall_Normal()override;
	bool Start();
	void Render(RenderContext& rc);
};

