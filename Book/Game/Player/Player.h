#pragma once
class PlayerManagement;
class GameCamera;
class Ghost;
class Star;
class Game;
class Treasure;  
class GameCamera;
class Player : public IGameObject
{
public:
	enum EnPlayerState {
		m_enPlayer_Idle,		//待機中
		m_enPlayer_walk,		//歩く
		m_enPlayer_Run,			//走る
		m_enPlayer_Jump,		//ジャンプ
		m_enPlayer_Jumpend,		//ジャンプ終わり
		m_enPlayer_Change,		//切替
		m_enPlayer_2DChanging,	//2Dに切替中
		m_enPlayer_3DChanging,	//3Dに切替中
		m_enPlayer_Found,		//見つかる
		m_enPlayer_Caught,		//捕まった
		m_enPlayer_Catching,	//捕まっている
		m_enPlayer_DownStart,	//気絶スタート
		m_enPlayer_Down,		//気絶中
		m_enPlayer_DownEnd,		//気絶解除
		m_enPlayer_Clear,		//ゲームクリア
		m_enPlayer_GameOver,	//ゲームオーバー
		m_enPlayer3D_Throw,		//投げる
		m_enPlayer3D_Steal,		//盗む
		m_enPlayer3D_Push,		//押す

	};

	EnPlayerState m_playerState = m_enPlayer_Idle;				//待機状態

	Player();
	virtual ~Player();
	virtual bool Start();
public:
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



	/// <summary>
	///透明ポジションの設定
	/// </summary>
	/// <returns></returns>
	void SetGhostPosition(const Vector3 ghostpos)
	{
		m_setGhostpos = ghostpos;
	}

	const Vector3 GetGhostPosition()const
	{
		return m_setGhostpos;
	}

	/// <summary>
	/// 透明ブロックの当たり判定
	/// </summary>
	void GhostHit();
	std::vector<Vector3> m_ghostpositions;
	bool m_ghostHit = true;										//壁に埋まったかを感知するブロックに当たったかどうか

	bool m_Player_Act = true;									//trueだったら行動可能falseだったら行動できない
	
	void SetMoveSpeed(const Vector3 speed)
	{
		m_moveSpeed = speed;
	}

protected:
	virtual void Update();
	void Move();
	void Jump();
	void Rotation();
	/// <summary>
	/// 捕まった判定
	/// </summary>
	void PlayerCatch();

	virtual void Throw()=0;
	virtual void Animation()=0;
	void Animation3D();
	void Animation2D();
	/// <summary>
	/// アイテムの切替
	/// </summary>
	void ItemChange();

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
	virtual void ProcessIdleStateTransition()=0;
	
	/// <summary>
	/// 移動ステートの遷移処理
	/// </summary>
	virtual void ProcessWalkStateTransition()=0;
	
	/// <summary>
	/// ダッシュステートの遷移処理
	/// </summary>
	virtual void ProcessRunStateTransition()=0;
	
	/// <summary>
	/// ジャンプステートの遷移処理
	/// </summary>
	virtual void ProcessJumpStateTransition()=0;
	
	/// <summary>
	/// ジャンプ終わりステートの遷移処理
	/// </summary>
	virtual void ProcessJumpendStateTransition()=0;
	
	/// <summary>
	/// 変更ステートの遷移処理
	/// </summary>
	virtual void ProcessChangeStateTransition()=0;

	/// <summary>
	/// 投げるステートの遷移処理
	/// </summary>
	virtual void ProcessThrowStateTransition()=0;

	/// <summary>
	/// 盗むステートの遷移処理
	/// </summary>
	virtual void ProcessStealStateTransition()=0;

	/// <summary>
	/// 見つかるステートの遷移処理
	/// </summary>
	virtual void ProcessFoundStateTransition()=0;

	/// <summary>
	/// 気絶ステートの遷移処理
	/// </summary>
	virtual void ProcessDownStartStateTransition()=0;

	/// <summary>
	/// 気絶ステートの遷移処理
	/// </summary>
	virtual void ProcessDownStateTransition()=0;

	/// <summary>
	/// 捕まるステートの遷移処理
	/// </summary>
	virtual void ProcessCaughtStateTransition()=0;

	/// <summary>
	/// 捕まった後の処理
	/// </summary>
	virtual void ProcessCatchingStateTransition() = 0;


	/// <summary>
	/// ゲームクリアステートの遷移処理
	/// </summary>
	virtual void ProcessClearStateTransition()=0;

	/// <summary>
	/// ゲームオーバーステートの遷移処理
	/// </summary>
	virtual void ProcessGameOverStateTransition()=0;

protected:
	//アニメーション
	enum EnAnimationClip {
		m_enAnimationClip_Idle,				//待機アニメーション
		m_enAnimationClip_Walk,				//歩きアニメーション
		m_enAnimationClip_Run,				//走るアニメーション
		m_enAnimationClip_Jump,				//ジャンプアニメーション
		m_enAnimationClip_Jumpend,			//ジャンプ終わりアニメーション
		m_enAnimationClip_Down,				//ダウンアニメーション
		m_enAnimationClip_DownEnd,			//ダウン終了
		m_enAnimationClip_Throw,			//投げるアニメーション
		m_enAnimationClip_CaughtStart,		//捕まった時のスタートアニメーション
		m_enAnimationClip_Caught,			//捕まった時のアニメーション
		m_enAnimationClip_Num,				//アニメーションの数
	};
	AnimationClip m_animationClips[m_enAnimationClip_Num];

	

	
	//アイテムの切替
	enum EnItemState
	{
		m_enItem_No,			//何も持っていない
		m_enItem_Flash,			//閃光弾			
		m_enItem_SoundBom		//音爆弾
	};
	EnItemState m_enItemState = m_enItem_Flash;
	
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
	CollisionObject* m_collisionObject = nullptr;				//コリジョン

	GameCamera* m_gamecamera=nullptr;								//ゲームカメラ
	PlayerManagement* m_playerManagement=nullptr;				//プレイヤー管理
	Star* m_star = nullptr;										//星
	Treasure* m_treasure = nullptr;
	Ghost* m_ghost = nullptr;
	Game* m_game = nullptr;


	Vector3 m_ghostPosition = Vector3::Zero;
	Vector3 m_setGhostpos=Vector3::Zero;

	float m_downTime = 3.0f;									//気絶時間

	bool m_playerCaught = true;

};