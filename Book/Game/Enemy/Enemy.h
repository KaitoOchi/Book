#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

namespace
{
	const int TIMER_NUM = 4;
}

class PlayerManagement;
class Gage;
class Game;
class Enemy :public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();

	bool Start();

	/// <summary>
	/// アニメーション
	/// </summary>
	void Animation();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// 壁と衝突したかどうかの処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <returns></returns>
	bool WallAndHit(Vector3 pos);
	/// <summary>
	/// 回転処理
	/// </summary>
	/// <param name="rot">自身が向かうベクトル</param>
	void Rotation(Vector3 rot);
	/// <summary>
	/// ナビメッシュを作成する
	/// </summary>
	void Nav(Vector3 pos);
	/// <summary>
	/// 巡回行動
	/// </summary>
	void Act_Craw();
	/// <summary>
	/// 追跡行動
	/// </summary>
	void Act_Tracking();
	/// <summary>
	/// 接近行動
	/// </summary>
	void Act_Access();
	/// <summary>
	/// 突進行動
	/// </summary>
	/// <param name="time">突進するまでのチャージ時間</param>
	void Act_Charge(float time);
	/// <summary>
	/// 敵を呼ぶ行動
	/// </summary>
	void Act_Call();
	/// <summary>
	/// 呼ばれた時の行動
	/// </summary>
	void Act_Called();
	/// <summary>
	/// 視野角内にプレイヤーが存在しないときの行動
	/// </summary>
	/// <returns></returns>
	bool Act_CallEnd();
	/// <summary>
	/// 見失ったときの処理
	/// </summary>
	void Act_Loss();
	/// <summary>
	/// 一定以内には近づかないための処理
	/// </summary>
	void Act_Limit();
	/// <summary>
	/// 閃光弾が当たったときの処理
	/// </summary>
	void Act_HitFlashBullet();
	/// <summary>
	/// 音爆弾が当たったときの処理
	/// </summary>
	bool Act_HitSoundBullet();
	/// <summary>
	/// 行動停止
	/// </summary>
	/// <param name="time">停止する時間</param>
	/// <param name="i">使用するタイマーを指定</param>
	/// <returns></returns>
	bool Act_Stop(float time,int i);
	/// <summary>
	/// プレイヤーを発見する処理
	/// </summary>
	/// <returns></returns>
	void Act_SeachPlayer();
	/// <summary>
	/// プレイヤーを確保する処理
	/// </summary>
	/// <returns></returns>
	bool Act_CatchPlayer();
	/// <summary>
	/// プレイヤーを見失った時の処理
	/// </summary>
	void Act_MissingPlayer();

	void SpotLight_New(Vector3 position,int num);
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);
	void VigilanceCount();				//

	enum EnemyType
	{
		Normal,
		Charge,
		Search,
		Clear
	};
	EnemyType m_enemyType;

	// エネミーのアニメーションステート
	// 継承した派生クラスでアニメーションを読み込み、関数を呼ぶと再生されます。
	enum EnAnimationClip
	{
		m_enAnimation_Idle,		// 待機
		m_enAnimation_Walk,		// 歩く
		m_enAnimation_Run,		// 走る
		m_enAnimation_Attack,	// 攻撃
		m_enAnimation_Damege,	// 被弾
		m_enAnimation_Dizzy,	// 閃光弾を受けたとき
		m_enAnimation_Loss,		// プレイヤーを見失った時
		m_enAnimation_Num
	};
	// アニメーションステート
	AnimationClip m_enAnimationClips[m_enAnimation_Num];
	
	// アニメーション再生用ステート
	enum EnAnimationState
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		DAMEGE,
		DIZZY,
		LOSS
	};
	EnAnimationState m_enAnimationState = IDLE;

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
		CONFUSION,		// 閃光弾にあたったとき
		LISTEN,			// 音爆弾を使用したとき
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
	/// <param name="CONFUSION">閃光弾にあたったとき</param>
	/// <param name="LISTEN">音爆弾を使用したとき</param>
	/// <param name="CATCH">捕獲</param>
	EnEnemyActState m_ActState = CRAW;

	void SearchPass(EnEnemyActState state);

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
	/// プレイヤーを確保したかどうか返す
	/// </summary>
	/// <returns></returns>
	bool GetChachPlayerFlag() {
		return m_ChachPlayerFlag;
	}

	/// <summary>
	/// 閃光弾の被弾フラグを設定
	/// </summary>
	/// <param name="">被弾したかどうかどうか判定する。trueなら被弾したと判定</param>
	void SetHitFlashBullet(bool b) {
		m_HitFlashBulletFlag = b;
	};

	/// <summary>
	/// 閃光弾に当たったかどうか返す
	/// </summary>
	/// <returns>被弾したかどうかどうか判定する。trueなら被弾したと判定</returns>
	bool GetHitFlushBullet() {
		return m_HitFlashBulletFlag;
	}

	/// <summary>
	/// 音爆弾の被弾フラグを設定
	/// </summary>
	/// <param name="">被弾したかどうかどうか判定する。trueなら被弾したと判定</param>
	void SetHitSoundBullet(bool b) {
		m_HitSoundBulletFlag = b;
	};

	/// <summary>
	/// 音爆弾に当たったかどうか返す
	/// </summary>
	/// <returns>被弾したかどうかどうか判定する。trueなら被弾したと判定</returns>
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

	// スポットライトの番号を教える
	void SetSpotLigNum(int num) {
		m_spotNum = num;
	}

	/// <summary>
	/// 描画するかどうか決定する。trueのとき描画しない
	/// </summary>
	/// <param name="flag"></param>
	void SetNotDrawFlag(bool flag) {
		m_NotDrawFlag = flag;
	}

	/// <summary>
	/// 行動パターンを戻す
	/// </summary>
	void SetTrueChangeDefaultFlag() {
		m_ChangeDefaultFlag = true;
	}

	//------------------------------------------
	//エネミーの前方向を求める
	const Vector3 GetFoward()const
	{
		return m_forward;
	}

	Vector3 m_foward=Vector3::AxisZ;
	//--------------------------------------------

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

	PlayerManagement* m_playerManagement = nullptr;
	Gage* m_gage = nullptr;
	Game* m_game = nullptr;

	EffectEmitter* m_soundEffect = nullptr;		// エフェクト

	CharacterController m_characterController;	// キャラクターコントローラー
	SphereCollider m_sphereCollider;			// スフィアコライダー

	FontRender m_fontRender;				// フォントレンダー

	Vector3 m_position = Vector3::Zero;		// エネミーの座標
	Vector3 m_forward = Vector3::AxisZ;		// エネミーの前方向
	Vector3 m_scale = Vector3::One;			// スケール
	Vector3 m_playerPos = Vector3::Zero;	// プレイヤーの座標
	Vector3 m_playerPos2 = Vector3::Zero;	// 突進用。プレイヤーの座標
	Vector3 m_playerPos3 = Vector3::Zero;	// 見失った時用。プレイヤーの座標
	Vector3 m_enemyPos = Vector3::Zero;		// 突進用。自身の座標
	Vector3 m_sumPos = Vector3::Zero;		// 総移動距離
	Vector3 m_setPos = Vector3::Zero;		// 集合する座標
	Vector3 m_itemPos = Vector3::Zero;		// アイテムの座標

	Quaternion m_rotation = Quaternion::Identity;	// 回転

	ModelRender m_enemyRender;				//エネミーモデル
	SpotLight m_spotLight;					//スポットライト

	bool m_HitFlashBulletFlag = false;		// 閃光弾が当たったかどうか
	bool m_HitSoundBulletFlag = false;		// 音爆弾
	bool m_FindPlayerFlag = false;			// プレイヤーの座標を参照するフラグ
	bool m_CalculatedFlag = false;			// 突進用フラグ
	bool m_CountFlag = false;				// カウントするフラグ
	bool m_ChachPlayerFlag = false;			// プレイヤーを確保したかどうか
	bool m_TrakingPlayerFlag = false;		// プレイヤーを追いかけるフラグ

	bool m_NotDrawFlag = false;				// 描画するかどうか
	bool m_ChangeDefaultFlag = false;		// デフォルトに切り替えるかどうか

	bool m_efectDrawFlag[3];				// エフェクト描画フラグ

	/// <summary>
	/// 0が閃光弾。1が巡回。2が突進用。3がプレイヤーを見失った時の処理
	/// </summary>
	float m_addTimer[TIMER_NUM];

	float m_NaviTimer = 0.0f;				// ナビメッシュ用のタイマー
	float m_move = 1.0f;
	float m_Vicount;						//警戒度を一定回数増やす

	int m_spotNum = 0;						// スポットライトの個数
};