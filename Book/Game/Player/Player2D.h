#pragma once
#include "Player.h"
class PlayerManagement;
class Player3D;
class Player2D:public Player
{
public:
	Player2D();
	~Player2D()override;
	bool Start()override;
	void Update()override;
	void PlayerChang();
	void Rotation2D();
	void Throw()override;
	/// <summary>
	/// キャラコンの作成の関数
	/// </summary>
	void CreatCharcon();
	void Animation()override;
	void Render(RenderContext& rc)override;
private:
	int i = 0;
	int j = 0;
	Vector3 m_InputRot=Vector3::Zero;
	Quaternion m_rot;
	PlayerManagement* m_playerManagement = nullptr;
	Player3D* m_player3D = nullptr;
private:
	/// <summary>
	/// 待機ステートの遷移処理
	/// </summary>
	void ProcessIdleStateTransition()override;
	/// <summary>
	/// 移動ステートの遷移処理
	/// </summary>
	void ProcessWalkStateTransition()override;
	/// <summary>
	/// ダッシュステートの遷移処理
	/// </summary>
	void ProcessRunStateTransition()override;
	/// <summary>
	/// ジャンプステートの遷移処理
	/// </summary>
	void ProcessJumpStateTransition()override;
	/// <summary>
	/// ジャンプ終わりステートの遷移処理
	/// </summary>
	void ProcessJumpendStateTransition()override;
	/// <summary>
	/// 変更ステートの遷移処理
	/// </summary>
	void ProcessChangeStateTransition()override;
	/// <summary>
	/// 投げるステートの遷移処理
	/// </summary>
	void ProcessThrowStateTransition()override;
	/// <summary>
	/// 盗むステートの遷移処理
	/// </summary>
	void ProcessStealStateTransition()override;
	/// <summary>
	/// 見つかるステートの遷移処理
	/// </summary>
	void ProcessFoundStateTransition()override;
	
	/// <summary>
	/// 気絶ステートの遷移処理
	/// </summary>
	void ProcessDownStateTransition()override;
	/// <summary>
	/// 捕まるステートの遷移処理
	/// </summary>
	void ProcessCaughtStateTransition()override;

	/// <summary>
	/// 捕まった後の処理
	/// </summary>
	void ProcessCatchingStateTransition()override;

	/// <summary>
	/// ゲームクリアステートの遷移処理
	/// </summary>
	void ProcessClearStateTransition()override;
	/// <summary>
	/// ゲームオーバーステートの遷移処理
	/// </summary>
	void ProcessGameOverStateTransition()override;
};

