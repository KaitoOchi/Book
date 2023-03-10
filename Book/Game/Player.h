#pragma once


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
	};
	Player();
	~Player();
	bool Start();
protected:
	void Update();
	void Move();
	void Jump();
	void Rotation();
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
	
	Vector3 m_moveSpeed=Vector3::Zero;//移動速度
	Vector3 m_Lstic = Vector3::Zero;//左ステック
	Vector3 m_position = Vector3(0.0f, 0.0f, 0.0f);//初期座標
	Vector3 m_forward = Vector3::AxisZ;//プレイヤーの正面ベクトル
	
	float m_walk = 20.0f;//歩き時の乗算量
	float m_run = 40.0f;//走り時の乗算量
	float m_jumpvolume = 150.0f;//ジャンプ量
	float angle=0;//回転角度
	
	Quaternion m_rotation;//回転
	CharacterController m_characon;//キャラコン
	EnPlayerState m_playerState = m_enPlayer_Idle;//待機状態
};