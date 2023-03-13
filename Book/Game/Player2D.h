#pragma once
#include "Player.h"
class Player3D;
class Player2D:public Player
{
public:
	Player2D();
	~Player2D();
	bool Start();
	void Update(bool m_newcharacon);
	void PlayerChang();
	void Animation();
	void Render(RenderContext& rc);
private:
	int i = 0;
	Player3D* m_player3D=nullptr;//3DÉvÉåÉCÉÑÅ[
};

