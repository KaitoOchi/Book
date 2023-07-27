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
	/// <summary>
	/// エネミーの種類。
	/// </summary>
	enum EnemyType
	{
		TYPE_NORMAL,	// 通常。
		TYPE_CHARGE,	// 突進。
		TYPE_SEARCH,	// 索敵。
		TYPE_CLEAR		// 透明。
	};

	/// <summary>
	/// エネミーの行動ステート。
	/// </summary>
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

protected:
	/// <summary>
	/// アニメーション再生用ステート。
	/// </summary>
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
	/// エネミーのアニメーション。
	/// </summary>
	/// 継承した派生クラスでアニメーションを読み込み、関数を呼ぶと再生されます。
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

	/// <summary>
	/// タイマーステート。
	/// </summary>
	enum EnTimerState
	{
		m_TimerState_HitByaFlashbang,		// 閃光弾を受けたときの再行動時間
		m_TimerState_StayOnThePath,			// 巡回時のパスに留まる時間
		m_TimerState_UntilTheCharge,		// 突進を行うまでの待機時間
		m_TimerState_MissingPlayer,			// プレイヤーを見失った時の待機時間
		m_TimerState_HitByaSoundbang,		// 音爆弾を使用された時の到達を諦めるまでの時間
		m_TimerState_Num
	};

	/// <summary>
	/// エフェクトステート。
	/// </summary>
	enum EnEffectState
	{
		m_EffectState_Star,					// ☆
		m_EffectState_ExclamationPoint,		// ！(感嘆符)
		m_EffectState_QuestionMark,			// ？(疑問符)
		m_EffectState_Num
	};

private:
	/// <summary>
	/// パス移動用の構造体。
	/// </summary>
	struct Point
	{
		Vector3		s_position;				// ポイントの座標
		int			s_number;				// ポイントの番号
	};

	/// <summary>
	/// パスステート。
	/// </summary>
	enum EnPathState
	{
		m_PathState_VerticalMovement,		// 縦移動
		m_PathState_MoveSideways,			// 横移動
		m_PathState_ClockwiseRotation,		// 右回り
	};

public:
	Enemy();
	virtual ~Enemy() = 0;
	bool Start();

public:
	/// <summary>
	/// 座標を設定する
	/// </summary>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// 座標を取得する
	/// </summary>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// 回転を取得する
	/// </summary>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/// <summary>
	/// スケールを設定する
	/// </summary>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// スケールを取得する
	/// </summary>
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/// <summary>
	/// プレイヤーを確保したかどうか返す
	/// </summary>
	/// <returns></returns>
	const bool GetChachPlayerFlag() const
	{
		return m_isChachPlayer;
	}

	/// <summary>
	/// プレイヤーを追跡しているかどうか返す
	/// </summary>
	/// <returns>trueなら追跡している</returns>
	const bool GetTrackingPlayerFlag() const
	{
		return m_isTrackingPlayer;
	}

	/// <summary>
	/// 閃光弾の被弾フラグを設定
	/// </summary>
	/// <param name="">被弾したかどうかどうか判定する。trueなら被弾したと判定</param>
	void SetHitFlashBulletFlag(const bool flag)
	{
		m_hitFlashBullet = flag;
	};

	/// <summary>
	/// 閃光弾に当たったどうかのフラグを取得する
	/// </summary>
	/// <returns>trueなら当たった</returns>
	const bool GetHitFlashBulletFlag() const
	{
		return m_hitFlashBullet;
	}

	/// <summary>
	/// 音爆弾の被弾フラグを設定
	/// </summary>
	/// <param name="">被弾したかどうかどうか判定する。trueなら被弾したと判定</param>
	void SetHearedSoundBulletFlag(const bool flag)
	{
		m_hearedSoundBullet = flag;
	};

	/// <summary>
	/// 音を聞いたかどうかのフラグを取得する
	/// </summary>
	/// <returns>trueなら聞いた</returns>
	const bool GetHearedSoundBulletFlag() const
	{
		return m_hearedSoundBullet;
	}

	/// <summary>
	/// アイテムの座標を渡す
	/// </summary>
	/// <returns></returns>
	void SetItemPos(const Vector3& position)
	{
		m_itemPosition = position;
	}

	/// <summary>
	/// 描画するかどうか決定する
	/// </summary>
	/// <param name="flag">trueのとき描画しない</param>
	void SetNotDrawFlag(const bool flag)
	{
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
	const bool GetNotDrawFlag() const
	{
		return m_canDraw;
	}

	/// <summary>
	/// エネミーの前方向を求める。
	/// </summary>
	/// <param name="state"></param>
	const Vector3 GetFoward() const
	{
		return m_forward;
	}

	/// <summary>
	/// 動かすかどうか決める
	/// </summary>
	/// <param name="active">trueなら動けない</param>
	void SetActiveFlag(const bool active)
	{
		m_canActive = active;
	}

	/// <summary>
	///  動かすかどうかの取得
	/// </summary>
	/// <param name="m_activeFlag">trueなら動けない</param>	
	const bool GetActiveFlag() const
	{
		return m_canActive;
	}

	/// <summary>
	/// スポットライトを取得。
	/// </summary>
	SpotLight& GetSpotLight()
	{
		return m_spotLight;
	}

	/// <summary>
	/// スポットライトの番号を教える
	/// </summary>
	/// <returns></returns>
	void SetSpotLigNum(int number)
	{
		m_spotNum = number;
	}

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
	/// 現在の行動パターンを取得する
	/// </summary>
	/// <returns></returns>
	const EnEnemyActionState GetEnemyActionState() const
	{
		return m_enActionState;
	}

	/// <summary>
	/// 種類を設定する
	/// </summary>
	void SetEnemyType(const EnemyType type)
	{
		m_enemyType = type;
	}

	/// <summary>
	/// エネミーの巡回パターンを指定
	/// </summary>
	/// <param name="0">	：縦				</param>
	/// <param name="1">	：横				</param>
	/// <param name="2">	：右回り(正方形)	</param>
	/// <param name="3">	：左回り(正方形)	</param>
	void SpecifyPath(int pathNumber);

	/// <summary>
	/// call実行時の集合する座標を渡す
	/// </summary>
	void SetGatherPosition(const Vector3& position)
	{
		m_gatherPosition = position;
	}

	/// <summary>
	/// 現在の行動パターンを設定する
	/// </summary>
	/// <param name="state"></param>
	void SetEnemyActionState(const EnEnemyActionState state)
	{
		m_enActionState = state;
	}

protected:
	/// <summary>
	/// エフェクトの描画をするかどうかのフラグを設定する
	/// </summary>
	/// <param name="flag">trueなら描画した</param>
	/// <param name="number">配列番号</param>
	void SetEffectDrawFlag(const bool flag, const int number)
	{
		m_existsEfectDraw[number] = flag;
	}

	/// <summary>
	/// タイマーをリセットする
	/// </summary>
	/// <param name="number">リセットするタイマー</param>
	void ReSetAddTimer(const int number)
	{
		m_addTimer[number] = 0.0f;
	}

	/// <summary>
	/// エフェクトを描画したかどうかのフラグを設定する
	/// </summary>
	/// <param name="number">設定するフラグ</param>
	/// <param name="flag">trueなら描画した</param>
	void SetEffectDrawFlag(const int number, const bool flag)
	{
		m_existsEfectDraw[number] = flag;
	}

	/// <summary>
	/// 再生するアニメーションを設定する
	/// </summary>
	void SetEnemyAnimationState(const EnAnimationState state)
	{
		m_enAnimationState = state;
	}

protected:
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
	bool WallAndHit(const Vector3& pos);
	/// <summary>
	/// 回転処理
	/// </summary>
	/// <param name="rot">自身が向かうベクトル</param>
	void Rotation(const Vector3& rot);
	/// <summary>
	/// ナビメッシュを作成する処理
	/// </summary>
	void CreateNavimesh(const Vector3& pos);
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
	void Action_GoLocationListenSound(const Vector3& tergetPosition);
	/// <summary>
	/// 行動停止
	/// </summary>
	/// <param name="time">停止する時間</param>
	/// <param name="timerNumber">使用するタイマーを指定</param>
	/// <returns></returns>
	bool Action_StopMove(const float time, const int timerNumber);
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

protected:
	ModelRender							m_enemyModelRender;							// エネミーモデル
	SpotLight							m_spotLight;								// スポットライト
	CharacterController					m_characterController;						// キャラクターコントローラー
	Game*								m_game = nullptr;							// ゲーム
	PlayerManagement*					m_playerManagement = nullptr;				// プレイヤーマネジメント
	EffectEmitter*						m_Effect = nullptr;							// エフェクト
	std::vector<Point>					m_pointList;								// ポイント構造体の配列
	Point*								m_point = nullptr;							// ポイント構造体のポインタ、現在の目的地になる
	Vector3								m_position = Vector3::Zero;					// エネミーの座標
	Vector3								m_forward = Vector3::AxisZ;					// エネミーの前方向
	Vector3								m_scale = Vector3::One;						// エネミーのスケール
	Vector3								m_chargeTergetPosition = Vector3::Zero;		// 突進用。プレイヤーの座標
	Vector3								m_playerMissiongPosition = Vector3::Zero;	// 見失った時用。プレイヤーの座標
	Vector3								m_sumPosition = Vector3::Zero;				// 総移動距離
	Vector3								m_gatherPosition = Vector3::Zero;			// 集合する座標
	Vector3								m_itemPosition = Vector3::Zero;				// アイテムの座標
	Quaternion							m_rotation = Quaternion::Identity;			// エネミーの回転
	AnimationClip						m_enAnimationClips[m_enAnimation_Num];		// アニメーションステート

private:
	SphereCollider						m_sphereCollider;							// スフィアコライダー
	TknFile								m_tknFile;									// tknファイル
	PhysicsStaticObject					m_bgObject;									// 静的物理オブジェクト
	nsAI::NaviMesh						m_nvmMesh;									// ナビメッシュ
	nsAI::Path							m_path;										// パス
	nsAI::PathFinding					m_pathFiding;								// パスを探す
	Gage*								m_gage = nullptr;							// 警戒度ゲージ
	EnAnimationState					m_enAnimationState;							// 再生するアニメーションを指定するステート
	EnEnemyActionState					m_enActionState;							// エネミーの行動パターンステート。
	EnemyType							m_enemyType;								// エネミーの種類。
	std::array<bool, m_EffectState_Num>	m_existsEfectDraw;							// エフェクトを描画したかどうかのフラグ。trueのとき描画した
	std::array<float, m_TimerState_Num>	m_addTimer;									// タイマー。
	bool								m_hitFlashBullet = false;					// 閃光弾が当たったかどうか
	bool								m_hearedSoundBullet = false;				// 音爆弾が当たったかどうか
	bool								m_shouldCount = false;						// 発見回数をカウントするかどうか
	bool								m_isTrackingPlayer = false;					// プレイヤーを追跡しているかどうか
	bool								m_isChachPlayer = false;					// プレイヤーを確保したかどうか
	bool								m_canDraw = false;							// 描画するかどうか
	bool								m_canActive = false;						// 最初から動けるかそうか
	int									m_spotNum = 0;								// スポットライトの個数
	float								m_naviTimer = 0.0f;							// ナビメッシュ用のタイマー
	float								m_addVicount = 0.0f;						// 警戒度を一定回数増やす
};