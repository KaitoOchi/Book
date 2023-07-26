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
	void LoadAnimation();
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
	void SpecifyPath(int pathNumber);
	/// <summary>
	/// ナビメッシュを作成する処理
	/// </summary>
	void CreateNavimesh(Vector3 pos);
	/// <summary>
	/// 巡回行動
	/// </summary>
	void Action_CrawPath();
	/// <summary>
	/// 追跡行動
	/// </summary>
	void Action_TrackingPlayer();
	/// <summary>
	/// 見失ったときの処理
	/// </summary>
	void Action_MissingPlayer();
	/// <summary>
	/// プレイヤーを見失った後の処理
	/// </summary>
	void Action_MoveMissingPosition();
	/// <summary>
	/// プレイヤーを見失った後の処理
	/// </summary>
	void Action_SearchMissingPlayer();
	/// <summary>
	/// 閃光弾が当たったときの処理
	/// </summary>
	void Action_HitFlashBullet();
	/// <summary>
	/// 音が聞こえた場所に行く処理
	/// </summary>
	/// <param name="pos">目標地点</param>
	void Action_GoLocationListenSound(Vector3 tergetPosition);
	/// <summary>
	/// 行動停止
	/// </summary>
	/// <param name="time">停止する時間</param>
	/// <param name="timerNumber">使用するタイマーを指定</param>
	/// <returns></returns>
	bool Action_StopMove(float time,int timerNumber);
	/// <summary>
	/// プレイヤーを発見する処理
	/// </summary>
	/// <returns></returns>
	void Action_SeachPlayer();
	/// <summary>
	/// プレイヤーを確保する処理
	/// </summary>
	/// <returns></returns>
	bool Action_CatchPlayer();
	/// <summary>
	/// スポットライトを生成
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="num">スポットライトの番号</param>
	void SpotLight_New(Vector3 position,int num);
	/// <summary>
	/// 索敵処理
	/// </summary>
	/// <param name="lightrotaition">回転</param>
	/// <param name="lightpos">座標</param>
	void SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos);
	/// <summary>
	/// 警戒度の増加処理
	/// </summary>
	void VigilanceCount();
	/// <summary>
	/// ☆のエフェクトを生成
	/// </summary>
	void Efect_Dizzy();
	/// <summary>
	/// ！のエフェクトを生成
	/// </summary>
	void Efect_FindPlayer();
	/// <summary>
	/// ？のエフェクトを生成
	/// </summary>
	void Efect_MissingPlayer();

	// ----------------------------------------------------------

	// エネミーの種類
	enum EnemyType
	{
		TYPE_NORMAL,	// 通常
		TYPE_CHARGE,	// 突進
		TYPE_SEARCH,	// 索敵
		TYPE_CLEAR		// 透明
	};
	/// <summary>
	/// 種類を設定する
	/// </summary>
	void SetEnemyType(EnemyType type)
	{
		m_enemyType = type;
	}

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

	/// <summary>
	/// 再生するアニメーションを設定する
	/// </summary>
	void SetEnemyAnimationState(const EnAnimationState state)
	{
		m_enAnimationState = state;
	}

	// エネミーの行動パターン
	enum EnEnemyActionState
	{
		m_ActionState_Craw,					// 巡回
		m_ActionState_Tracking,				// 追跡
		m_ActionState_Search,				// 索敵
		m_ActionState_Move_MissingPositon,	// 見失った座標まで移動する
		m_ActionState_Search_MissingPlayer,	// 見失ったプレイヤーを探す
		m_ActionState_Call_AroundEnemy,		// 周りの敵を呼ぶ
		m_ActionState_Called,				// CALL時にSearch以外が実行
		m_ActionState_Charge,				// 突進
		m_ActionState_ChargeEnd,			// 突進終了
		m_ActionState_BackBasedOn,			// 巡回状態に戻る
		m_ActionState_Dizzy,				// 閃光弾にあたったとき
		m_ActionState_Listen,				// 音爆弾を使用したとき
		m_ActionState_CatchPlayer,			// 捕獲した
	};

	/// <summary>
	/// 現在の行動パターンを設定する
	/// </summary>
	/// <param name="state"></param>
	void SetEnemyActionState(const EnEnemyActionState state) {
		m_enActionState = state;
	}

	/// <summary>
	/// 現在の行動パターンを取得する
	/// </summary>
	/// <returns></returns>
	const EnEnemyActionState GetEnemyActionState() const {
		return m_enActionState;
	}

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
		return m_isChachPlayer;
	}

	/// <summary>
	/// プレイヤーを追跡しているかどうか返す
	/// </summary>
	/// <returns>trueなら追跡している</returns>
	const bool GetTrackingPlayerFlag() const {
		return m_isTrackingPlayer;
	}

	/// <summary>
	/// エフェクトの描画をするかどうかのフラグを設定する
	/// </summary>
	/// <param name="flag">trueなら描画した</param>
	/// <param name="number">配列番号</param>
	void SetEffectDrawFlag(const bool flag,const int number) {
		m_existsEfectDraw[number] = flag;
	}

	/// <summary>
	/// 閃光弾の被弾フラグを設定
	/// </summary>
	/// <param name="">被弾したかどうかどうか判定する。trueなら被弾したと判定</param>
	void SetHitFlashBulletFlag(bool flag) {
		m_hitFlashBullet = flag;
	};

	/// <summary>
	/// 閃光弾に当たったどうかのフラグを取得する
	/// </summary>
	/// <returns>trueなら当たった</returns>
	const bool GetHitFlashBulletFlag() const {
		return m_hitFlashBullet;
	}

	/// <summary>
	/// 音爆弾の被弾フラグを設定
	/// </summary>
	/// <param name="">被弾したかどうかどうか判定する。trueなら被弾したと判定</param>
	void SetHearedSoundBulletFlag(bool flag) {
		m_hearedSoundBullet = flag;
	};

	/// <summary>
	/// 音を聞いたかどうかのフラグを取得する
	/// </summary>
	/// <returns>trueなら聞いた</returns>
	const bool GetHearedSoundBulletFlag()const {
		return m_hearedSoundBullet;
	}

	/// <summary>
	/// アイテムの座標を渡す
	/// </summary>
	/// <returns></returns>
	void SetItemPos(const Vector3& position) {
		m_itemPosition = position;
	}

	/// <summary>
	/// 描画するかどうか決定する
	/// </summary>
	/// <param name="flag">trueのとき描画しない</param>
	void SetNotDrawFlag(bool flag) {
		m_canDraw = flag;

		if (flag == false) {
			return;
		}

		// フラグをfalseにする
		m_hitFlashBullet = false;
		m_hearedSoundBullet = false;
		m_isTrackingPlayer = false;
		m_isChachPlayer = false;

		// ステートを巡回状態に戻す
		m_enActionState = m_ActionState_Craw;

		m_naviTimer = 0.0f;
		m_addTimer[m_TimerState_StayOnThePath] = 0.0f;
		m_addTimer[m_TimerState_MissingPlayer] = 0.0f;

		for (int i = 0; i < m_EffectState_Num; i++) {
			m_existsEfectDraw[i] = false;
		}
	}

	/// <summary>
	/// 描画するかどうかのフラグを取得する
	/// </summary>
	/// <returns>trueなら描画しない</returns>
	const bool GetNotDrawFlag() const {
		return m_canDraw;
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
		m_canActive = active;
	}

	/// <summary>
	///  動かすかどうかの取得
	/// </summary>
	/// <param name="m_activeFlag">trueなら動けない</param>	
	bool GetActiveFlag()
	{
		return m_canActive;
	}

	// スポットライトを渡す
	SpotLight& GetSpotLight() {
		return m_spotLight;
	}

	/// <summary>
	/// スポットライトの番号を教える
	/// </summary>
	/// <returns></returns>
	void SetSpotLigNum(int number) {
		m_spotNum = number;
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
		return m_enemyModelRender;
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

	/// <summary>
	/// タイマーをリセットする
	/// </summary>
	/// <param name="number">リセットするタイマー</param>
	void ReSetAddTimer(const int number) {
		m_addTimer[number] = 0.0f;
	}

	/// <summary>
	/// エフェクトを描画したかどうかのフラグを設定する
	/// </summary>
	/// <param name="number">設定するフラグ</param>
	/// <param name="flag">trueなら描画した</param>
	void SetEffectDrawFlag(const int number,const bool flag) {
		m_existsEfectDraw[number] = flag;
	}

	/// <summary>
	/// 集合する座標を渡す
	/// </summary>
	void SetGatherPosition(const Vector3 position) {
		m_gatherPosition = position;
	}

private:
	SphereCollider				m_sphereCollider;							// スフィアコライダー

	// パス移動用のポイント構造体
	struct Point {
		Vector3					s_position;									// ポイントの座標
		int						s_number;									// ポイントの番号
	};

	// ナビメッシュ
	TknFile						m_tknFile;									// tknファイル
	PhysicsStaticObject			m_bgObject;									// 静的物理オブジェクト
	nsAI::NaviMesh				m_nvmMesh;									// ナビメッシュ
	nsAI::Path					m_path;										// パス
	nsAI::PathFinding			m_pathFiding;								// パスを探す

	Gage*						m_gage = nullptr;							// 警戒度ゲージ

protected:
	Game*						m_game = nullptr;							// ゲーム
	PlayerManagement*			m_playerManagement = nullptr;				// プレイヤーマネジメント
	EffectEmitter*				m_Effect = nullptr;							// エフェクト

	// パス
	std::vector<Point>			m_pointList;								// ポイント構造体の配列
	Point*						m_point = nullptr;							// ポイント構造体のポインタ、現在の目的地になる

	CharacterController			m_characterController;						// キャラクターコントローラー

	Vector3						m_position = Vector3::Zero;					// エネミーの座標
	Vector3						m_forward = Vector3::AxisZ;					// エネミーの前方向
	Vector3						m_scale = Vector3::One;						// エネミーのスケール
	Vector3						m_chargeTergetPosition = Vector3::Zero;		// 突進用。プレイヤーの座標
	Vector3						m_playerMissiongPosition = Vector3::Zero;	// 見失った時用。プレイヤーの座標
	Vector3						m_sumPosition = Vector3::Zero;				// 総移動距離
	Vector3						m_gatherPosition = Vector3::Zero;			// 集合する座標
	Vector3						m_itemPosition = Vector3::Zero;				// アイテムの座標

	Quaternion					m_rotation = Quaternion::Identity;			// エネミーの回転
	ModelRender					m_enemyModelRender;							// エネミーモデル
	SpotLight					m_spotLight;								// スポットライト

	AnimationClip				m_enAnimationClips[m_enAnimation_Num];		// アニメーションステート

	enum EnTimerState {
		m_TimerState_HitByaFlashbang,				// 閃光弾を受けたときの再行動時間
		m_TimerState_StayOnThePath,					// 巡回時のパスに留まる時間
		m_TimerState_UntilTheCharge,				// 突進を行うまでの待機時間
		m_TimerState_MissingPlayer,					// プレイヤーを見失った時の待機時間
		m_TimerState_HitByaSoundbang,				// 音爆弾を使用された時の到達を諦めるまでの時間
		m_TimerState_Num
	};

	enum EnEffectState {
		m_EffectState_Star,							// ☆
		m_EffectState_ExclamationPoint,				// ！(感嘆符)
		m_EffectState_QuestionMark,					// ？(疑問符)
		m_EffectState_Num
	};

private:
	enum EnPathState {
		m_PathState_VerticalMovement,				// 縦移動
		m_PathState_MoveSideways,					// 横移動
		m_PathState_ClockwiseRotation,				// 右回り
	};

	// 再生するアニメーションを指定するステート
	EnAnimationState			m_enAnimationState;

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
	EnEnemyActionState			m_enActionState;

	// エネミーの種類
	EnemyType					m_enemyType;

	/// <summary>
	/// エフェクトを描画したかどうかのフラグ。trueのとき描画した
	/// </summary>
	/// <param name="0">	：☆のエフェクト	</param>
	/// <param name="1">	：!のエフェクト		</param>
	/// <param name="2">	：?のエフェクト		</param>
	std::array<bool, 3>			m_existsEfectDraw;

	/// <summary>
	/// </summary>
	/// <param name="0">	：閃光弾を受けたときの再行動時間	</param>
	/// <param name="1">	：巡回時のパスに留まる時間			</param>
	/// <param name="2">	：突進を行うまでの待機時間			</param>
	/// <param name="3">	：プレイヤーを見失った時の待機時間	</param>
	/// <param name="4">	：音爆弾を使用された時の到達を諦めるまでの時間	</param>
	std::array<float, 5>		m_addTimer;

	bool						m_hitFlashBullet = false;			// 閃光弾が当たったかどうか
	bool						m_hearedSoundBullet = false;		// 音爆弾が当たったかどうか
	bool						m_shouldCount = false;				// 発見回数をカウントするかどうか
	bool						m_isTrackingPlayer = false;			// プレイヤーを追跡しているかどうか
	bool						m_isChachPlayer = false;			// プレイヤーを確保したかどうか
	bool						m_canDraw = false;					// 描画するかどうか
	bool						m_canActive = false;				// 最初から動けるかそうか
	float						m_naviTimer = 0.0f;					// ナビメッシュ用のタイマー
	float						m_addVicount = 0.0f;				// 警戒度を一定回数増やす
	int							m_spotNum = 0;						// スポットライトの個数
};