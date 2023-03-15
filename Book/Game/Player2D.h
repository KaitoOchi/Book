#pragma once
#include "Player.h"
class PlayerManagement;
class Player2D:public Player
{
public:
	Player2D();
	~Player2D();
	bool Start();
	void Update();
	void PlayerChang();
	/// <summary>
	/// ƒLƒƒƒ‰ƒRƒ“‚Ìì¬‚ÌŠÖ”
	/// </summary>
	void CreatCharcon();
	void Animation();
	void Render(RenderContext& rc);
private:
	int i = 0;
	int j = 0;
	PlayerManagement* m_playerManagement = nullptr;
};

