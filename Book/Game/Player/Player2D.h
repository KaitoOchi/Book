#pragma once
#include "Player.h"
class PlayerManagement;
class Player3D;
class Player2D:public Player
{
public:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Change,
		enAnimationClip_Num
	};
	AnimationClip m_changeAnimation[enAnimationClip_Num];

public:
	Player2D();
	~Player2D()override;
	bool Start()override;
	void Update()override;
	void PlayerChang();
	void Throw()override;
	/// <summary>
	/// キャラコンの作成の関数
	/// </summary>
	void CreatCharcon();
	void Animation()override;
	void Render(RenderContext& rc)override;
private:
	int					m_addValue = 0;
	int					m_playAnimationSpeed = 0;
	int					m_walkAnimationSpeed = 30;								//歩くときのアニメーション速度
	int					m_jumpAnimationSpeed = 90;								//ジャンプ時のアニメーション速度
	float				m_runAnimationSpeed = 3.0f;								//ダッシュ時のアニメーション速度

	bool				m_chactState2D = false;

	Vector3				m_InputRot=Vector3::Zero;
	Quaternion			m_rot = Quaternion::Identity;

	PlayerManagement*	m_playerManagement = nullptr;
	Player3D*			m_player3D = nullptr;
	EffectEmitter*		m_smokeEffect = nullptr;

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

