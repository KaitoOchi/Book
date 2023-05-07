#pragma once
#include "Enemy.h"
class Enemy_Normal:public Enemy
{
public:
	Enemy_Normal();
	~Enemy_Normal();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Update_OnCraw();				// „‰ñ
	void Update_OnTracking();			// ’ÇÕ
	void Update_OnCalled();				// CALL‚ÉÀs
	void Update_OnBackBasedOn();		// „‰ñó‘Ô‚É–ß‚é
	void Update_OnConfusion();			// ‘MŒõ’e‚É“–‚½‚Á‚½‚Æ‚«
	void UpDate_OnListen();				// ‰¹”š’e‚ğg—p‚µ‚½‚Æ‚«
	void Update_OnCatch();				// •ßŠl

private:

	Enemy* enemy;
};