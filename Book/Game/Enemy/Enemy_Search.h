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
	/// 回転処理
	/// </summary>
	void Rotaition();
	/// <summary>
	/// 敵を呼ぶ行動
	/// </summary>
	void Action_CallAroundEnemy();
	/// <summary>
	/// 索敵
	/// </summary>
	void Update_OnSearch();
	/// <summary>
	/// 周りの敵を呼ぶ
	/// </summary>
	void Update_OnCallAroundEnemy();
	/// <summary>
	/// 閃光弾に当たったとき
	/// </summary>
	void Update_OnDizzy();
	/// <summary>
	/// プレイヤーを見失った時
	/// </summary>
	void Update_OnMissingPlayer();

private:
	bool	m_rotState = true;				// 回転の向きの切替
	float	m_rotTime = 2.0f;				// 回転を切り替えるタイマー
};