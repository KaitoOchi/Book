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

	void Update_OnSearch();				// 索敵
	void Update_OnCall();				// 周りの敵を呼ぶ
	void Update_OnConfusion();			// 閃光弾に当たったとき
	void Update_OnMissingPlayer();		// プレイヤーを見失った時
	void Update_OnEvent();				// イベント時の処理
		
private:
	ModelRender m_searchModelRender;	// ホイッスルを呼び出すモデル

	//SpotLight m_spotLight;				//スポットライト
	bool m_rotState=true;				//回転の向きの切替
	float m_rotTime = 2.0f;
};