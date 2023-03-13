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
		b = m_HitFlashBulletFlag;
	};

private:

	ModelRender m_modelRender;			// モデルレンダー
	FontRender m_fontRender;			// フォントレンダー

	// アニメーションクリップ
	enum EnEnemyAnimationClip
	{
		enAnimationClip_Idle,			// 待機
		enAnimationClip_Walk,			// 歩く
		enAnimationClip_Run,			// 走る
		enAnimationClip_Attack,			// 攻撃
		enAnimationClip_Damege,			// 被弾
		enAnimationClip_Num
	};

	AnimationClip m_animationClips[enAnimationClip_Num];

	Vector3 m_position;

	bool m_FoundPlayerFlag = false;		// プレイヤーを発見したかどうか
	bool m_CatchPlayerFlag = false;		// プレイヤーを捕まえたかどうか
	bool m_MissigPlayerFlag = false;	// プレイヤーを見失ったかどうか
	bool m_HitFlashBulletFlag = false;	// 閃光弾が当たったかどうか
	bool m_ReturnToPassFlag = false;	// 直近のパスに戻ったかどうか

	float catchDecision = 10.0f;		// 捕まえたと認識する範囲
};

