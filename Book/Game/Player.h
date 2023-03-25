#pragma once
class PlayerManagement;
class GameCamera;
class Player : public IGameObject
{
public:
	enum EnPlayerState{
		m_enPlayer_Idle,//待機中
		m_enPlayer_walk,//歩く
		m_enPlayer_Run,//走る
		m_enPlayer_Jump,//ジャンプ
		m_enPlayer_Jumpend,//ジャンプ終わり
		m_enPlayer_Change,//切替
		m_enPlayer_2DChanging,//2Dに切替中
		m_enPlayer_3DChanging,//3Dに切替中
		m_enPlayer_Found,//見つかる
		m_enPlayer_Caught,//捕まった
		m_enPlayer_Clear,//ゲームクリア
		m_enPlayer_GameOver,//ゲームオーバー
		m_enPlayer3D_Throw,//投げる
		m_enPlayer3D_Steal,//盗む
		m_enPlayer3D_Push,//押す

	};
	Player();
	virtual ~Player();
	virtual bool Start();
	//座標の取得
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	//座標の取得
	void SetPosition(const Vector3& m_pos)
	{
		m_position = m_pos;
	}
	// 外部から更新をする
	void ModelRenderUpdate() {
		m_modelRender->SetPosition(m_position);
		m_modelRender->Update();
	}

	//キャラコンの取得
	CharacterController* GetCharacon()
	{
		return m_characon;
	}

	//コリジョンの取得
	CollisionObject* GetCollision()
	{
		return m_collisionObject;
	}

protected:
	virtual void Update();
	void Move();
	void Jump();
	void Rotation();
	virtual void Animation();
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	void ManageState();
	/// <summary>
	/// 共通のステート遷移処理
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移処理
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 移動ステートの遷移処理
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// ダッシュステートの遷移処理
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// ジャンプステートの遷移処理
	/// </summary>
	void ProcessJumpStateTransition();
	/// <summary>
	/// ジャンプ終わりステートの遷移処理
	/// </summary>
	void ProcessJumpendStateTransition();
	/// <summary>
	/// 変更ステートの遷移処理
	/// </summary>
	void ProcessChangeStateTransition();
	/// <summary>
	/// 2Dに切替中ステートの遷移処理
	/// </summary>
	void Process2DChangingStateTransition();
	/// <summary>
	/// 3Dに切替中ステートの遷移処理
	/// </summary>
	void Process3DChangingStateTransition();
	/// <summary>
	/// 投げるステートの遷移処理
	/// </summary>
	void ProcessThrowStateTransition();
	/// <summary>
	/// 盗むステートの遷移処理
	/// </summary>
	void ProcessStealStateTransition();
	/// <summary>
	/// 押すステートの遷移処理
	/// </summary>
	void ProcessPushStateTransition();
	/// <summary>
	/// 見つかるステートの遷移処理
	/// </summary>
	void ProcessFoundStateTransition();
	/// <summary>
	/// 捕まるステートの遷移処理
	/// </summary>
	void ProcessCaughtStateTransition();
	/// <summary>
	/// ゲームクリアステートの遷移処理
	/// </summary>
	void ProcessClearStateTransition();
	/// <summary>
	/// ゲームオーバーステートの遷移処理
	/// </summary>
	void ProcessGameOverStateTransition();
	
	bool m_characonState = true;								//キャラコンを作るかどうか

	Vector3 m_moveSpeed=Vector3::Zero;							//移動速度
	Vector3 m_Lstic = Vector3::Zero;							//左ステック
	Vector3 m_position = Vector3(0.0f, 0.0f, 0.0f);				//初期座標
	Vector3 m_forward = Vector3::AxisZ;							//プレイヤーの正面ベクトル
	
	float angle=0;												//回転角度
	
	ModelRender *m_modelRender=nullptr;							//3Dモデル
	nsK2EngineLow::Texture m_player2D[14];						//テクスチャ
	Quaternion m_rotation;										//回転
	CharacterController *m_characon;							//キャラコン
	EnPlayerState m_playerState = m_enPlayer_Idle;				//待機状態
	CollisionObject* m_collisionObject = nullptr;							//コリジョン

	GameCamera* gamecamera=nullptr;								//ゲームカメラ
	PlayerManagement* m_playerManagement=nullptr;				//プレイヤー管理
};