#pragma once
#include "Player.h"
class PlayerManagement;
class Player3D :public Player
{
public:
	Player3D();
	~Player3D()override;
	bool Start()override;
	void Update()override;
	void Animation()override;
	void Throw();
	void PlayerChang();
	void Render(RenderContext& rc);
	/// <summary>
	/// ƒLƒƒƒ‰ƒRƒ“‚Ìì¬‚ÌŠÖ”
	/// </summary>
	void CreatCharcon();

	void SetPushPosition(const Vector3 pushpos)
	{
		m_pushPosition = pushpos;
	}
	const Vector3 GetPushPosition()const
	{
		return m_pushPosition;
	}

private:
	PlayerManagement* m_playerManagement = nullptr;

	//“§–¾À•W‚É“®‚©‚·
	Vector3 m_pushPosition = Vector3::Zero;
	Vector3 m_pushRotPos = Vector3::Zero;

	float m_pushRot;
	float senkeiPos = 0.0f;
	float senkeiTime = 1.0f;
};

