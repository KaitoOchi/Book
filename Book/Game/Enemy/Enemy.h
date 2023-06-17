#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class PlayerManagement;
class Gage;
class Game;
class Treasure;
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
	/// エネミーの巡回パターンを指定
	/// </summary>
	/// <param name="0">	：縦				</param>
	/// <param name="1">	：横				</param>
	/// <param name="2">	：右回り(正方形)	</param>
	/// <param name="3">	：左回り(正方形)	</param>
	/// <param name="4">	：右に直角			</param>
	/// <param name="6">	：右に直角			</param>
	/// <param name="7">	：右回り(長方形)	</param>
	/// <param name="8">	：左回り(長方形)	</param>
	void SpecifyPath(int pathNumber);

	/// <summary>
	/// ナビメッシュを作成する処理
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
	/// プレイヤーを見失った後の処理
	/// 見失った位置まで位置を移動する
	/// </summary>
	void Act_MoveMissingPosition();
	/// <summary>
	/// プレイヤーを見失った後の処理
	/// プレイヤーを探す
	/// </summary>
	void Act_SearchMissingPlayer();

	/// <summary>
	/// 閃光弾が当たったときの処理
	/// </summary>
	void Act_HitFlashBullet();
	/// <summary>
	/// 音が聞こえた場所に行く処理
	/// </summary>
	/// <param name="pos">目標地点</param>
	void Act_GoLocationListenSound(Vector3 tergetPos);

	/// <summary>
	/// 行動停止
	/// </summary>
	/// <param name="time">停止する時間</param>
	/// <param name="timerNumber">使用するタイマーを指定</param>
	/// <returns></returns>
	bool Act_Stop(float time,int timerNumber);

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

	void SpotLight_New(Vector3 position,int num);
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);
	void VigilanceCount();				//

	void Efect_Dizzy();				// ☆のエフェクトを生成
	void Efect_FindPlayer();		// ！のエフェクトを生成
	void Efect_MissingPlayer();		// ？のエフェクトを生成

	// ----------------------------------------------------------

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
		m_enAnimation_Idle,					// 待機
		m_enAnimation_Walk,					// 歩く
		m_enAnimation_Run,					// 走る
		m_enAnimation_Attack,				// 攻撃
		m_enAnimation_Damage,				// 被弾
		m_enAnimation_Dizzy,				// 閃光弾を受けたとき
		m_enAnimation_Loss,					// プレイヤーを見失った時
		m_enAnimation_Call,					// 他のエネミーを呼ぶ
		m_enAnimation_Num
	};
	// アニメーションステート
	AnimationClip m_enAnimationClips[m_enAnimation_Num];
	
	// アニメーション再生用ステート
	enum EnAnimationState
	{
		m_enAnimationState_Idle,			// 待機
		m_enAnimationState_Walk,			// 歩く
		m_enAnimationState_Run,				// 走る
		m_enAnimationState_Attack,			// 攻撃
		m_enAnimationState_Damage,			// 被弾
		m_enAnimationState_Dizzy,			// 閃光弾を受けたとき
		m_enAnimationState_Loss,			// プレイヤーを見失った時
		m_enAnimationState_Call				// 他のエネミーを呼ぶ
	};
	EnAnimationState m_enAnimationState;

	// エネミーの行動パターン
	enum EnEnemyActState
	{
		m_ActState_Craw,					// 巡回
		m_ActState_Tracking,				// 追跡
		m_ActState_Search,					// 索敵
		m_ActState_Move_MissingPositon,		// 見失った座標まで移動する
		m_ActState_Search_MissingPlayer,	// 見失ったプレイヤーを探す
		m_ActState_Call_AroundEnemy,		// 周りの敵を呼ぶ
		m_ActState_Called,					// CALL時にSearch以外が実行
		m_ActState_Charge,					// 突進
		m_ActState_ChargeEnd,				// 突進終了
		m_ActState_BackBasedOn,				// 巡回状態に戻る
		m_ActState_Dizzy,					// 閃光弾にあたったとき
		m_ActState_Listen,					// 音爆弾を使用したとき
		m_ActState_CatchPlayer,				// 捕獲した
	};

	/// <summary>
	/// エネミーの行動パターン
	/// </summary>
	/// <param name="CRAW">					：巡回							</param>
	/// <param name="TRACKING">				：追跡							</param>
	/// <param name="SEARCH">				：待機							</param>
	/// <param name="MISSING_MOVEPOSITON">	：見失った座標まで移動した		</param>
	/// <param name="MISSING_SEARCHPLAYER">	：見失ったプレイヤーを探す		</param>
	/// <param name="CALL">					：周りの敵を呼ぶ				</param>
	/// <param name="CALLED">				：CALL時にSearch以外が実行		</param>
	/// <param name="CHARGE">				：突進							</param>
	/// <param name="BACKBASEDON">			：巡回状態に戻る				</param>
	/// <param name="CONFUSION">			：閃光弾にあたったとき			</param>
	/// <param name="LISTEN">				：音爆弾を使用したとき			</param>
	/// <param name="CATCH">				：捕獲							</param>
	EnEnemyActState m_ActState;

	// ----------------------------------------------------------

	/// <summary>
	/// 座標を設定する
	/// </summary>
	void SetPosition(const Vector3& position) {
		m_position = position;
	}

	/// <summary>
	/// 座標を取得する
	/// </summary>
	const Vector3& GetPosition() const {
		return m_position;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	void SetRotation(const Quaternion& rotation) {
		m_rotation = rotation;
	}

	/// <summary>
	/// 回転を取得する
	/// </summary>
	const Quaternion& GetRotation() const {
		return m_rotation;
	}

	/// <summary>
	/// スケールを設定する
	/// </summary>
	void SetScale(const Vector3& scale) {
		m_scale = scale;
	}

	/// <summary>
	/// スケールを取得する
	/// </summary>
	const Vector3& GetScale() const {
		return m_scale;
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
	void SetHitFlashBullet(bool flag) {
		m_HitFlashBulletFlag = flag;
	};

	/// <summary>
	/// 音爆弾の被弾フラグを設定
	/// </summary>
	/// <param name="">被弾したかどうかどうか判定する。trueなら被弾したと判定</param>
	void SetHearedSoundBullet(bool flag) {
		m_HearedSoundBulletFlag = flag;
	};

	/// <summary>
	/// アイテムの座標を渡す
	/// </summary>
	/// <returns></returns>
	void SetItemPos(const Vector3& position) {
		m_itemPos = position;
	}

	/// <summary>
	/// スポットライトの番号を教える
	/// </summary>
	/// <returns></returns>
	void SetSpotLigNum(int number) {
		m_spotNum = number;
	}

	/// <summary>
	/// 描画するかどうか決定する。trueのとき描画しない
	/// </summary>
	/// <param name="flag"></param>
	void SetNotDrawFlag(bool flag) {
		m_NotDrawFlag = flag;

		if (flag == false) {
			return;
		}

		// フラグをfalseにする
		m_HitFlashBulletFlag = false;
		m_HearedSoundBulletFlag = false;
		m_TrackingPlayerFlag = false;
		m_ChachPlayerFlag = false;

		// ステートを巡回状態に戻す
		m_ActState = m_ActState_Craw;

		m_NaviTimer = 0.0f;
		m_addTimer[1] = 0.0f;
		m_addTimer[3] = 0.0f;

		m_efectDrawFlag[0] = false;
		m_efectDrawFlag[1] = false;
		m_efectDrawFlag[2] = false;
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

	/// <summary>
	///  動かすかどうかの取得
	/// </summary>
	/// <param name="m_activeFlag">trueなら動けない</param>	
	bool GetActiveFlag()
	{
		return m_activeFlag;
	}

	// スポットライトを渡す
	SpotLight GetSpotLight() {
		return m_spotLight;
	}

	//--------------------------------------------

	/// <summary>
	/// キャラクターコントローラーを取得
	/// </summary>
	CharacterController& GetCharCon()
	{
		return m_characterController;
	}

	/// <summary>
	/// モデルレンダーを取得する
	/// </summary>
	ModelRender& GetModelRender()
	{
		return m_enemyRender;
	}


	//--------------------------------------------

	/// <summary>
	/// エフェクトを取得。
	/// </summary>
	/// <returns></returns>
	EffectEmitter* GetEffect()
	{
		if (m_Effect != nullptr) {
			return m_Effect;
		}
		return nullptr;
	}

protected:
	// パス移動用のポイント構造体
	struct Point {
		Vector3					s_position;							// ポイントの座標
		int						s_number;							// ポイントの番号
	};
	// パス
	std::vector<Point>			m_pointList;						// ポイント構造体の配列
	Point*						m_point = nullptr;					// ポイント構造体のポインタ、現在の目的地になる

	// ナビメッシュ
	TknFile						m_tknFile;							// tknファイル
	PhysicsStaticObject			m_bgObject;							// 静的物理オブジェクト
	nsAI::NaviMesh				m_nvmMesh;							// ナビメッシュ
	nsAI::Path					m_path;								// パス
	nsAI::PathFinding			m_pathFiding;						// パスを探す

	PlayerManagement*			m_playerManagement = nullptr;		// プレイヤーマネジメント
	Gage*						m_gage = nullptr;					// 警戒度ゲージ
	Game*						m_game = nullptr;					// ゲーム
	EffectEmitter*				m_Effect = nullptr;					// エフェクト

	CharacterController			m_characterController;				// キャラクターコントローラー
	SphereCollider				m_sphereCollider;					// スフィアコライダー

	Vector3						m_position = Vector3::Zero;			// エネミーの座標
	Vector3						m_forward = Vector3::AxisZ;			// エネミーの前方向
	Vector3						m_scale = Vector3::One;				// エネミーのスケール

	Vector3						m_chargeTergetPosition = Vector3::Zero;		// 突進用。プレイヤーの座標
	Vector3						m_playerMissiongPosition = Vector3::Zero;	// 見失った時用。プレイヤーの座標

	Vector3						m_sumPos = Vector3::Zero;			// 総移動距離
	Vector3						m_setPos = Vector3::Zero;			// 集合する座標
	Vector3						m_itemPos = Vector3::Zero;			// アイテムの座標
	Vector3						m_chargeDiff = Vector3::Zero;		// 突進の移動量

	Quaternion					m_rotation = Quaternion::Identity;	// エネミーの回転

	ModelRender					m_enemyRender;						//エネミーモデル

	SpotLight					m_spotLight;						//スポットライト

protected:
	/// <summary>
	/// エフェクトを描画したかどうかのフラグ。trueのとき描画した
	/// </summary>
	/// <param name="0">	：☆のエフェクト	</param>
	/// <param name="1">	：!のエフェクト		</param>
	/// <param name="2">	：?のエフェクト		</param>
	std::array<bool, 3>			m_efectDrawFlag;

	/// <summary>
	/// </summary>
	/// <param name="0">	：閃光弾を受けたときの再行動時間	</param>
	/// <param name="1">	：巡回時のパスに留まる時間			</param>
	/// <param name="2">	：突進を行うまでの待機時間			</param>
	/// <param name="3">	：プレイヤーを見失った時の待機時間	</param>
	/// <param name="4">	：音爆弾を使用された時の到達を諦めるまでの時間	</param>
	std::array<float, 5>		m_addTimer;

	bool						m_HitFlashBulletFlag = false;		// 閃光弾が当たったかどうか
	bool						m_HearedSoundBulletFlag = false;	// 音爆弾が		〃
	bool						m_CountFlag = false;				// 発見回数をカウントするかどうか決定する
	bool						m_TrackingPlayerFlag = false;		// プレイヤーを追いかけるフラグ
	bool						m_ChachPlayerFlag = false;			// プレイヤーを確保したかどうか
	bool						m_CalculatedFlag = false;			// 突進用フラグ。一度だけ参照を行う
	bool						m_NotDrawFlag = false;				// 描画するかどうか
	bool						m_activeFlag = false;				// 最初から動けるかそうか
	bool						m_SearchFlag = false;				// 警戒度が最大の時に一度だけ実行する

private:
	float						m_NaviTimer = 0.0f;					// ナビメッシュ用のタイマー
	float						m_Chargemove = 1.0f;				// 突進ステート時に乗算しているスカラー
	float						m_Vicount;							// 警戒度を一定回数増やす

	int							m_spotNum = 0;						// スポットライトの個数
};