#pragma once
#include "Enemy.h"
class Enemy_Search:public Enemy
{
public:
	Enemy_Search();
	~Enemy_Search()override;

	bool Start();
	void Update();
	void Rotaition();
	void Render(RenderContext& rc);

	void Update_OnSearch();				// õ“G
	void Update_OnCallAroundEnemy();	// ü‚è‚Ì“G‚ğŒÄ‚Ô
	void Update_OnDizzy();				// ‘MŒõ’e‚É“–‚½‚Á‚½‚Æ‚«
	void Update_OnMissingPlayer();		// ƒvƒŒƒCƒ„[‚ğŒ©¸‚Á‚½
		
private:
	bool m_rotState=true;				//‰ñ“]‚ÌŒü‚«‚ÌØ‘Ö
	float m_rotTime = 2.0f;
};