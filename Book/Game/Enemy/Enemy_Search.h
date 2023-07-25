#pragma once
#include "Enemy.h"
class Enemy_Search:public Enemy
{
public:
	Enemy_Search();
	~Enemy_Search()override;

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ‰ñ“]ˆ—
	/// </summary>
	void Rotaition();
	/// <summary>
	/// “G‚ğŒÄ‚Ôs“®
	/// </summary>
	void Action_CallAroundEnemy();
	/// <summary>
	/// õ“G
	/// </summary>
	void Update_OnSearch();
	/// <summary>
	/// ü‚è‚Ì“G‚ğŒÄ‚Ô
	/// </summary>
	void Update_OnCallAroundEnemy();
	/// <summary>
	/// ‘MŒõ’e‚É“–‚½‚Á‚½‚Æ‚«
	/// </summary>
	void Update_OnDizzy();
	/// <summary>
	/// ƒvƒŒƒCƒ„[‚ğŒ©¸‚Á‚½
	/// </summary>
	void Update_OnMissingPlayer();

private:
	bool	m_rotState = true;				// ‰ñ“]‚ÌŒü‚«‚ÌØ‘Ö
	float	m_rotTime = 2.0f;				// ‰ñ“]‚ğØ‚è‘Ö‚¦‚éƒ^ƒCƒ}[
};