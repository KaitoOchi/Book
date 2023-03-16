#pragma once

class PlayerManagement;
class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();

	bool Start();

	bool SeachPlayer();					// プレイヤーを発見する処理
	bool CatchPlayer();					// プレイヤーを確保する処理
	void HitFlashBullet();				// 閃光弾が当たったときの処理
	void HitAfterFlashBullet();			// 閃光弾が当たった後の処理

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
		m_enEnemyActState_Tracking,		// 追跡
		m_enEnemyActState_Waiting,		// 待機
	};
	// 行動ステート
	EnEnemyActState m_enEnemyActState = m_enEnemyActState_Craw;

	/// <summary>
	/// 座標を設定する
	/// </summary>
	void SetPosition(Vector3 pos) {
		m_position = pos;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	void SetRotation(Quaternion rot) {
		m_rotation = rot;
	}

	/// <summary>
	/// スケールを設定する
	/// </summary>
	void SetScale(Vector3 sca) {
		m_scale = sca;
	}

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
	const Vector3 GetPosition() const {
		return m_position;
	}

	const Vector3 GetScale() const {
		return m_scale;
	}

	const Quaternion GetRotation() const {
		return m_rotation;
	}

protected:
	PlayerManagement* m_playerManagement = nullptr;

	CharacterController m_characterController;
	FontRender m_fontRender;				// フォントレンダー

	Vector3 m_position = Vector3::Zero;		// エネミーの座標
	Vector3 m_forward = Vector3::AxisZ;		// エネミーの前方向
	Vector3 m_scale = Vector3::One;			// スケール
	Quaternion m_rotation;					// 回転

	bool MissigPlayerFlag = false;			// プレイヤーを見失ったかどうか
	bool HitFlashBulletFlag = false;		// 閃光弾が当たったかどうか
	bool ReturnToPassFlag = false;			// 直近のパスに戻ったかどうか
};