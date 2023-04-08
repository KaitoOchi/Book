#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class PlayerManagement;
class Enemy :public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();

	bool Start();

	bool WallAndHit(Vector3 pos);		// 壁と衝突したかどうかの処理
	void Rotation(Vector3 rot);			// 回転処理
	void Act_Craw();					// 巡回行動
	void Act_Tracking();				// 追跡行動
	void Act_Access();					// 接近行動
	void Act_Charge(float time);		// 突進行動
	void Act_Loss();					// 見失ったときの処理
	void Act_Limit();					// 一定以内には近づかないための処理
	void Act_HitFlashBullet();			// 閃光弾が当たったときの処理
	bool Act_Stop(float time);			// 行動停止
	bool Act_SeachPlayer();				// プレイヤーを発見する処理
	bool Act_CatchPlayer();				// プレイヤーを確保する処理
	void SpotLight_New(Vector3 position);
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);

	// エネミーのアニメーションステート
	// 継承した派生クラスでアニメーションを読み込み、関数を呼ぶと再生されます。
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
	// 必要なければ削除してください。
	enum EnEnemyActState
	{
		m_enEnemyActState_Craw,			// 巡回
		m_enEnemyActState_Tracking,		// 追跡
		m_enEnemyActState_Waiting,		// 待機
		m_enEnemyActState_Confusion		// 錯乱
	};
	// 行動ステート
	EnEnemyActState m_enEnemyActState = m_enEnemyActState_Craw;


	// 指定できるパス移動
	enum EnEnemyPassState
	{
		LINE_VERTICAL,					// 縦
		LINE_HORIZONTAL,				// 横
		SQUARE_RIGHT,					// 右回り(正方形)
		SQUARE_LEFT,					// 左回り(正方形)
		ANGLE_RIGHT,					// 右に直角
		ANGLE_LEFT,						// 左に直角
		RECTANGLE_RIGHT,				// 右回り(長方形)
		RECTANGLE_LEFT,					// 左回り(長方形)
	};
	EnEnemyPassState PassState;
	/// <summary>
	/// エネミーの巡回パターンを指定
	/// </summary>
	/// <param name="0">縦</param>
	/// <param name="1">横</param>
	/// <param name="2">右回り(正方形)</param>
	/// <param name="3">左回り(正方形)</param>
	/// <param name="4">右に直角</param>
	/// <param name="6">右に直角</param>
	/// <param name="7">右回り(長方形)</param>
	/// <param name="8">左回り(長方形)</param>
	void Pass(int num);

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
		b = HitFlashBulletFlag;
	};

	/// <summary>
	/// 座標を取得する
	/// </summary>
	const Vector3& GetPosition() const {
		return m_position;
	}

	/// <summary>
	/// スケールを取得する
	/// </summary>
	const Vector3& GetScale() const {
		return m_scale;
	}

	/// <summary>
	/// 回転を取得する
	/// </summary>
	const Quaternion& GetRotation() const {
		return m_rotation;
	}

protected:

	// パス移動用のポイント構造体
	struct Point {
		Vector3 s_position;					// ポイントの座標
		int s_number;						// ポイントの番号
	};

	TknFile m_tknFile;						// tknファイル
	PhysicsStaticObject m_bgObject;			// 静的物理オブジェクト
	nsAI::NaviMesh m_nvmMesh;				// ナビメッシュ
	nsAI::Path m_path;						// パス
	nsAI::PathFinding m_pathFiding;			// パスを探す

	std::vector<Point> m_pointList;			// ポイント構造体の配列
	Point* m_point = nullptr;				// ポイント構造体のポインタ、現在の目的地になる

	PlayerManagement* m_playerManagement = nullptr;

	CharacterController m_characterController;

	SphereCollider m_sphereCollider;

	FontRender m_fontRender;				// フォントレンダー

	Vector3 m_position = Vector3::Zero;		// エネミーの座標
	Vector3 m_forward = Vector3::AxisZ;		// エネミーの前方向
	Vector3 m_scale = Vector3::One;			// スケール
	Quaternion m_rotation=Quaternion::Identity;	// 回転

	Vector3 m_playerPos = Vector3::Zero;	// プレイヤーの座標

	bool HitFlashBulletFlag = false;		// 閃光弾が当たったかどうか
	bool ChangeFlag = false;				// 追跡⇒巡回の切り替えフラグ
	bool FindPlayerFlag = false;
	bool CalculatedFlag = false;

	float addTimer = 0.0f;					// 加算するタイマー
	float NaviTimer = 0.0f;					// ナビメッシュ用のタイマー

	Vector3 playerPos = Vector3::Zero;		// 突進用
	Vector3 enemyPos = Vector3::Zero;

	ModelRender m_enemyRender;				//エネミーモデル
	SpotLight m_spotLight;					//スポットライト
};