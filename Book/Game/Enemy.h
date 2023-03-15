#pragma once

class PlayerManagement;
class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	bool SeachPlayer();					// プレイヤーを発見する処理
	bool CatchPlayer();					// プレイヤーを確保する処理
	bool HitFlashBullet();				// 閃光弾が当たったときの処理
	void HitAfterFlashBullet();			// 閃光弾が当たった後の処理
	void HeadToDestination();			// 目的地に向かう処理
	void Act();							// 行動パターン
	void Animation();					// アニメーション

	/// <summary>
	/// 被弾フラグ設定用。必要なければ消去
	/// </summary>
	/// <param name="">被弾したかどうかどうか判定する。trueなら被弾したと判定</param>
	void SetHitFlashBullet(bool b) {
		// 引数を渡す
		b = HitFlashBulletFlag;
	};

	/// <summary>
	/// 座標を取得する
	/// </summary>
	const Vector3 GetPosition() {
		return m_position;
	}

private:
	PlayerManagement* m_playerManagement = nullptr;

	CharacterController m_characterController;

	ModelRender m_modelRender;			// モデルレンダー
	FontRender m_fontRender;			// フォントレンダー

	// アニメーションクリップ
	enum EnEnemyAnimationClip
	{
		m_enAnimationClip_Idle,			// 待機
		m_enAnimationClip_Walk,			// 歩く
		m_enAnimationClip_Run,			// 走る
		m_enAnimationClip_Attack,		// 攻撃
		m_enAnimationClip_Damege,		// 被弾
		m_enAnimationClip_Num
	};
	// アニメーションクリップ
	AnimationClip m_animationClips[m_enAnimationClip_Num];


	// エネミーのアニメーションステート
	enum EnEnemyAnimationState
	{
		m_enEnemyAnimationState_Idle,	// 待機
		m_enEnemyAnimationState_Walk,	// 歩く
		m_enEnemyAnimationState_Run,	// 走る
		m_enEnemyAnimationState_Attack,	// 攻撃
		m_enEnemyAnimationState_Damege,	// 被弾
	};
	// アニメーションステート
	EnEnemyAnimationState m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;

	// エネミーの行動ステート
	enum EnEnemyActState
	{
		m_enEnemyActState_Craw,			// 巡回
		m_enEnemyActState_Tracking		// 追跡
	};
	// 行動ステート
	EnEnemyActState m_enEnemyActState = m_enEnemyActState_Craw;

	Vector3 m_position = Vector3::Zero;		// エネミーの座標
	Vector3 m_forward = Vector3::AxisZ;		// エネミーの前方向
	bool MissigPlayerFlag = false;			// プレイヤーを見失ったかどうか
	bool HitFlashBulletFlag = false;		// 閃光弾が当たったかどうか
	bool ReturnToPassFlag = false;			// 直近のパスに戻ったかどうか
};