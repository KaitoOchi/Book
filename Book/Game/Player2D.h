#pragma once
#include "Player.h"
class Player2D:public Player
{
public:
	Player2D();
	~Player2D();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	
private:
	int i = 0;
};

