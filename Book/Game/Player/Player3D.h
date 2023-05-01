#pragma once
#include "Player.h"
class PlayerManagement;
class FlashBom;
class SoundBom;
class Player3D :public Player
{
public:
	Player3D();
	~Player3D()override;
	bool Start()override;
	void Update()override;
	void Animation()override;
	void Throw()override;
	void PlayerChang();
	void Render(RenderContext& rc);
	/// <summary>
	/// キャラコンの作成の関数
	/// </summary>
	void CreatCharcon();

	void SetPushPosition(const Vector3 pushpos)
	{
		m_pushPosition = pushpos;
	}
	const Vector3 GetPushPosition()const
	{
		return m_pushPosition;
	}

private:
	PlayerManagement* m_playerManagement = nullptr;
	FlashBom* m_flashBom = nullptr;
	SoundBom* m_soundBom = nullptr;
	

	//アニメーションイベント用の関数
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	bool m_isThrowing = false;

	//透明座標に動かす
	Vector3 m_pushPosition = Vector3::Zero;
	Vector3 m_pushRotPos = Vector3::Zero;

	float m_pushRot;
	float senkeiPos = 0.0f;
	float senkeiTime = 1.0f;
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
	void ProcessDownStartStateTransition()override;
	/// <summary>
	/// 気絶ステートの遷移処理
	/// </summary>
	void ProcessDownStateTransition()override;
	/// <summary>
	/// 捕まるステートの遷移処理
	/// </summary>
	void ProcessCaughtStateTransition()override;
	/// <summary>
	/// ゲームクリアステートの遷移処理
	/// </summary>
	void ProcessClearStateTransition()override;
	/// <summary>
	/// ゲームオーバーステートの遷移処理
	/// </summary>
	void ProcessGameOverStateTransition()override;
};

