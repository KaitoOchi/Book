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
	void Rotation2D();
	/// <summary>
	/// ƒLƒƒƒ‰ƒRƒ“‚Ìì¬‚ÌŠÖ”
	/// </summary>
	void CreatCharcon();
	void Animation();
	void Render(RenderContext& rc);
private:
	int i = 0;
	int j = 0;
	Vector3 m_InputRot=Vector3::Zero;
	Quaternion m_rot;
	PlayerManagement* m_playerManagement = nullptr;
};

