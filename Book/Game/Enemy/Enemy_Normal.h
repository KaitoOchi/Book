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

	void Update_OnCraw();				// 巡回
	void Update_OnTracking();			// 追跡
	void Update_OnCalled();				// CALL時に実行
	void Update_OnBackBasedOn();		// 巡回状態に戻る
	void Update_OnConfusion();			// 閃光弾に当たったとき
	void UpDate_OnListen();				// 音爆弾を使用したとき
	void Update_OnCatch();				// 捕獲

private:

	Enemy* enemy;
};