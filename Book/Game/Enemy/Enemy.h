#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class PlayerManagement;
class GameUI;
class Game;
class Enemy :public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();

	bool Start();

	bool WallAndHit(Vector3 pos);		// 壁と衝突したかどうかの処理
	void Rotation(Vector3 rot);			// 回転処理
	void Nav(Vector3 pos);				// ナビメッシュを行う処理
	void Act_Craw();					// 巡回行動
	void Act_Tracking();				// 追跡行動
	void Act_Access();					// 接近行動
	void Act_Charge(float time);		// 突進行動
	void Act_Call();					// 敵を呼ぶ行動
	void Act_Called();					// 呼ばれた時の行動
	bool Act_CallEnd();					// 視野角内にプレイヤーが存在しないときの行動
	void Act_Loss();					// 見失ったときの処理
	void Act_Limit();					// 一定以内には近づかないための処理
	void Act_HitFlashBullet();			// 閃光弾が当たったときの処理
	bool Act_Stop(float time,int i);	// 行動停止
	bool Act_SeachPlayer();				// プレイヤーを発見する処理
	bool Act_CatchPlayer();				// プレイヤーを確保する処理
	void SpotLight_New(Vector3 position,int num);
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);
	void VigilanceCount();				//


	enum EnemyType
	{
		Normal,
		Charge,
		Search
	};
	EnemyType m_enemyType;

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

	// エネミーの行動パターン
	enum EnEnemyActState
	{
		CRAW,			// 巡回
		TRACKING,		// 追跡
		SEARCH,			// 索敵
		CALL,			// 周りの敵を呼ぶ
		CALLED,			// CALL時にSearch以外が実行
		CALLEND,		// 視野角内にプレイヤーが存在しないとき実行
		CHARGE,			// 突進
		BACKBASEDON,	// 巡回状態に戻る
		CONFUSION,		// 錯乱
		CATCH			// 捕獲
	};
	/// <summary>
	/// エネミーの行動パターン。switchで管理してください
	/// </summary>
	/// <param name="CRAW">巡回</param>
	/// <param name="TRACKING">追跡</param>
	/// <param name="SEARCH">待機</param>
	/// <param name="CALL">周りの敵を呼ぶ</param>
	/// <param name="CALLED">CALL時にSearch以外が実行</param>
	/// <param name="CALLEND">視野角内にプレイヤーが存在しないとき実行</param>
	/// <param name="CHARGE">突進</param>
	/// <param name="BACKBASEDON">巡回状態に戻る</param>
	/// <param name="CONFUSION">錯乱</param>
	/// <param name="CATCH">捕獲</param>
	EnEnemyActState m_ActState = CRAW;

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
		m_HitFlashBulletFlag = b;
	};

	/// <summary>
	/// 閃光弾に当たったかどうか返す
	/// </summary>
	/// <returns></returns>
	bool GetHitFlushBullet() {
		return m_HitFlashBulletFlag;
	}

	/// <summary>
	/// 被弾フラグ設定用。必要なければ消去
	/// </summary>
	/// <param name="">被弾したかどうかどうか判定する。trueなら被弾したと判定</param>
	void SetHitSoundBullet(bool b) {
		m_HitSoundBulletFlag = b;
	};

	/// <summary>
	/// アイテムの座標を設定する
	/// </summary>
	/// <returns></returns>
	bool GetHitSoundBullet() {
		return m_HitSoundBulletFlag;
	}

	/// <summary>
	/// アイテムの座標を渡す
	/// </summary>
	/// <returns></returns>
	bool SetItemPos(Vector3 pos) {
		m_itemPos = pos;
	}

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

	std::vector<Point> m_pointList;			// ポイント構造体の配列
	Point* m_point = nullptr;				// ポイント構造体のポインタ、現在の目的地になる

	TknFile m_tknFile;						// tknファイル
	PhysicsStaticObject m_bgObject;			// 静的物理オブジェクト
	nsAI::NaviMesh m_nvmMesh;				// ナビメッシュ
	nsAI::Path m_path;						// パス
	nsAI::PathFinding m_pathFiding;			// パスを探す

	std::vector<Enemy*> enemyList;			// エネミーのリスト

	PlayerManagement* m_playerManagement = nullptr;
	GameUI* m_gameUI = nullptr;
	Game* m_game = nullptr;

	CharacterController m_characterController;	// キャラクターコントローラー
	SphereCollider m_sphereCollider;			// スフィアコライダー

	FontRender m_fontRender;				// フォントレンダー

	Vector3 m_position = Vector3::Zero;		// エネミーの座標
	Vector3 m_forward = Vector3::AxisZ;		// エネミーの前方向
	Vector3 m_scale = Vector3::One;			// スケール
	Quaternion m_rotation=Quaternion::Identity;	// 回転

	Vector3 m_playerPos = Vector3::Zero;	// プレイヤーの座標

	bool m_HitFlashBulletFlag = false;		// 閃光弾が当たったかどうか
	bool m_HitSoundBulletFlag = false;		// 音爆弾
	bool m_FindPlayerFlag = false;			
	bool m_CalculatedFlag = false;			// 突進用フラグ

	float m_addTimer[3];					// 加算するタイマー。処理ごとに配列を作成
	float m_NaviTimer = 0.0f;				// ナビメッシュ用のタイマー
	float m_move = 1.0f;

	Vector3 m_playerPos2 = Vector3::Zero;	// 突進用。プレイヤーの座標
	Vector3 m_enemyPos = Vector3::Zero;		// 突進用。自身の座標
	Vector3 m_sumPos = Vector3::Zero;		// 総移動距離
	Vector3 m_setPos = Vector3::Zero;		// 集合する座標
	Vector3 m_itemPos = Vector3::Zero;		// アイテムの座標

	ModelRender m_enemyRender;				//エネミーモデル
	SpotLight m_spotLight;					//スポットライト

	float m_Vicount;						//警戒度を一定回数増やす
};