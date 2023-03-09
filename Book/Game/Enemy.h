#pragma once
class Enemy:public IGameObject
{
public:
	Enemy();
	~Enemy();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void MoveState();					// 行動パターンの処理
	void HeadToDestination();			// 目的地に向かう処理
	void Seach();						// 視野角の処理
	void CatchPlayer();					// プレイヤーを確保する処理
	void HitFlashBullet();				// 閃光弾が当たったときの処理

	// 被弾フラグ設定用。必要なければ消去
	// trueのとき被弾
	void SetHitFlashBullet(bool b) {
		// 引数を渡す
		b = HitFlashBulletFlag;
	};

private:

	ModelRender m_modelRender;			// モデルレンダー
	FontRender m_fontRender;			// フォントレンダー

	// アニメーションクリップ
	enum EnEnemyAnimationClip
	{
		enEnemyAnimationClip_Idle,		// 待機
		enEnemyAnimationClip_Walk,		// 歩く
		enEnemyAnimationClip_Run,		// 走る
		enEnemyAnimationClip_Attack,	// 攻撃
		enEnemyAnimationClip_Damege,	// 被弾
		enEnemyAnimationClip_Num
	};

	AnimationClip m_animationClips[enEnemyAnimationClip_Num];

	Vector3 m_position;

	bool FoundPlayerFlag = false;		// プレイヤーを発見したかどうか
	bool CatchPlayerFlag = false;		// プレイヤーを捕まえたかどうか
	bool MissigPlayerFlag = false;		// プレイヤーを見失ったかどうか
	bool HitFlashBulletFlag = false;	// 閃光弾が当たったかどうか
	bool ReturnToPassFlag = false;		// 直近のパスに戻ったかどうか
};

