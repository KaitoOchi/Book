#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class PlayerManagement;
class Gage;
class Game;
class Enemy :public IGameObject
{
public:
	Enemy();
	virtual ~Enemy()=0;

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
	void CreateNavimesh(Vector3 pos);
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
	/// 突進した後の行動処理
	/// </summary>
	void Act_ChargeEnd();
	/// <summary>
	/// 壁との衝突判定
	/// </summary>
	void Act_Charge_HitWall();
	/// <summary>
	/// 敵を呼ぶ行動
	/// </summary>
	void Act_Call();
	/// <summary>
	/// 見失ったときの処理
	/// </summary>
	void Act_Loss();
	/// <summary>
	/// 閃光弾が当たったときの処理
	/// </summary>
	void Act_HitFlashBullet();
	/// <summary>
	/// 音が聞こえた場所に行く処理
	/// </summary>
	/// <param name="pos">目標地点</param>
	void Act_GoLocationListenSound(Vector3 pos);
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
	/// 見失った位置まで位置を移動する
	/// </summary>
	void Act_MoveMissingPosition();
	/// <summary>
	/// プレイヤーを見失った時の処理
	/// プレイヤーを探す
	/// </summary>
	void Act_SearchMissingPlayer();

	void SpotLight_New(Vector3 position,int num);
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);
	void VigilanceCount();				//

	void Efect_Dizzy();
	void Efect_FindPlayer();
	void Efect_MissingPlayer();

	// エネミーの種類
	enum EnemyType
	{
		TYPE_NORMAL,
		TYPE_CHARGE,
		TYPE_SEARCH,
		TYPE_CLEAR
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
		CRAW,					// 巡回
		TRACKING,				// 追跡
		SEARCH,					// 索敵
		MISSING_MOVEPOSITON,	// 見失った座標まで移動した
		MISSING_SEARCHPLAYER,	// 見失ったプレイヤーを探す
		CALL,					// 周りの敵を呼ぶ
		CALLED,					// CALL時にSearch以外が実行
		CHARGE,					// 突進
		CHARGEEND,				// 突進終了
		BACKBASEDON,			// 巡回状態に戻る
		CONFUSION,				// 閃光弾にあたったとき
		LISTEN,					// 音爆弾を使用したとき
		CATCH					// 捕獲した
	};
	/// <summary>
	/// エネミーの行動パターン。switchで管理してください
	/// </summary>
	/// <param name="CRAW">					：巡回										</param>
	/// <param name="TRACKING">				：追跡										</param>
	/// <param name="SEARCH">				：待機										</param>
	/// <param name="MISSING_MOVEPOSITON">	：見失った座標まで移動した					</param>
	/// <param name="MISSING_SEARCHPLAYER">	：見失ったプレイヤーを探す					</param>
	/// <param name="CALL">					：周りの敵を呼ぶ							</param>
	/// <param name="CALLED">				：CALL時にSearch以外が実行					</param>
	/// <param name="CHARGE">				：突進										</param>
	/// <param name="BACKBASEDON">			：巡回状態に戻る							</param>
	/// <param name="CONFUSION">			：閃光弾にあたったとき						</param>
	/// <param name="LISTEN">				：音爆弾を使用したとき						</param>
	/// <param name="CATCH">				：捕獲										</param>
	EnEnemyActState m_ActState;

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
	/// <param name="0">	：縦				</param>
	/// <param name="1">	：横				</param>
	/// <param name="2">	：右回り(正方形)	</param>
	/// <param name="3">	：左回り(正方形)	</param>
	/// <param name="4">	：右に直角		</param>
	/// <param name="6">	：右に直角		</param>
	/// <param name="7">	：右回り(長方形)	</param>
	/// <param name="8">	：左回り(長方形)	</param>
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
	void SetHearedSoundBullet(bool b) {
		m_HearedSoundBulletFlag = b;
	};

	/// <summary>
	/// 音爆弾に当たったかどうか返す
	/// </summary>
	/// <returns>被弾したかどうかどうか判定する。trueなら被弾したと判定</returns>
	bool GetHitSoundBullet() {
		return m_HearedSoundBulletFlag;
	}

	/// <summary>
	/// アイテムの座標を渡す
	/// </summary>
	/// <returns></returns>
	void SetItemPos(Vector3 pos) {
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

	const int GetSpotLigNum()
	{
		return m_spotNum;
	}

	/// <summary>
	/// 描画するかどうか決定する。trueのとき描画しない
	/// </summary>
	/// <param name="flag"></param>
	void SetNotDrawFlag(bool b) {
		m_NotDrawFlag = b;

		if (b == false) {
			return;
		}

		// ステートを巡回状態に戻す
		m_ActState = CRAW;
		// 追跡・捕獲フラグをfalseにする
		m_TrackingPlayerFlag = false;
		m_ChachPlayerFlag = false;
	}

	//エネミーの前方向を求める
	const Vector3 GetFoward()const
	{
		return m_forward;
	}

	Vector3 m_foward=Vector3::AxisZ;

	/// <summary>
	/// 動かすかどうか決める
	/// </summary>
	/// <param name="active">trueなら動けない</param>
	void SetActiveFlag(bool active)
	{
		m_activeFlag = active;
	}

	const bool GetActiveFlag()
	{
		return m_activeFlag;
	}

	// スポットライトを渡す
	SpotLight GetSpotLight() {
		return m_spotLight;
	}

	//--------------------------------------------
	CharacterController& GetCharCon()
	{
		return m_characterController;
	}

	const int GetNumber()
	{
		return m_spotNum;
	}

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
	Vector3 m_playerChargePosition = Vector3::Zero;			// 突進用。プレイヤーの座標
	Vector3 m_playerMissionPosition = Vector3::Zero;		// 見失った時用。プレイヤーの座標
	Vector3 m_sumPos = Vector3::Zero;		// 総移動距離
	Vector3 m_setPos = Vector3::Zero;		// 集合する座標
	Vector3 m_itemPos = Vector3::Zero;		// アイテムの座標
	Vector3 m_chargeDiff = Vector3::Zero;	// 突進の移動量


	Quaternion m_rotation = Quaternion::Identity;		// 回転

	ModelRender m_enemyRender;				//エネミーモデル
	SpotLight m_spotLight;					//スポットライト

	bool m_HitFlashBulletFlag = false;		// 閃光弾が当たったかどうか
	bool m_HearedSoundBulletFlag = false;		// 音爆弾
	bool m_CountFlag = false;				// カウントするフラグ
	bool m_TrackingPlayerFlag = false;		// プレイヤーを追いかけるフラグ
	bool m_ChachPlayerFlag = false;			// プレイヤーを確保したかどうか
	bool m_CalculatedFlag = false;			// 突進用フラグ。一度だけ参照を行う
	bool m_NotDrawFlag = false;				// 描画するかどうか
	bool m_activeFlag = false;				//最初から動けるかそうか

	/// <summary>
	/// エフェクトを描画したかどうかのフラグ。trueのとき描画した
	/// </summary>
	/// <param name="0">	：☆のエフェクト	</param>
	/// <param name="1">	：!のエフェクト		</param>
	/// <param name="2">	：?のエフェクト		</param>
	std::array<bool, 3>m_efectDrawFlag;

	/// <summary>
	/// </summary>
	/// <param name="0">	：閃光弾を受けたときの再行動時間	</param>
	/// <param name="1">	：巡回時のパスに留まる時間			</param>
	/// <param name="2">	：突進を行うまでの待機時間			</param>
	/// <param name="3">	：プレイヤーを見失った時の待機時間	</param>
	std::array<float, 4>m_addTimer;

	float m_NaviTimer = 0.0f;				// ナビメッシュ用のタイマー
	float m_move = 1.0f;
	float m_Vicount;						//警戒度を一定回数増やす

	int m_spotNum = 0;						// スポットライトの個数
};