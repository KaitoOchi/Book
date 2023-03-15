#pragma once
#include "Player.h"
#include "Player2D.h"
#include "Player3D.h"
class Player2D;
class Player3D;
class PlayerManagement:public Player
{
public:
	PlayerManagement();
	~PlayerManagement();
	bool Start();
	void Update();
	void PlayerChange();
	/// <summary>
	/// ポジションの取得
	/// </summary>
	const Vector3& GetPosition()const
	{
		// 今アクティブなプレイヤーの座標を返す
		if (m_enMnanagementState == m_enPlayer_3DChanging) {
			return m_player3D->GetPosition();
		}
		else {
			return m_player2D->GetPosition();
		}
	}
	void  SetPosition(const Vector3& m_pos)
	{
		m_position = m_pos;
	}
	/// <summary>
	/// 共通のステート遷移処理
	/// </summary>
	void ProcessCommonStateTransition();
private:
	enum EnMnagementState
	{
		m_enPlayer_2DChanging,//2Dに切替中
		m_enPlayer_3DChanging,//3Dに切替中
	};
	EnMnagementState m_enMnanagementState = m_enPlayer_3DChanging;//３D状態
	Vector3 m_position = Vector3::Zero;
	Player2D* m_player2D = nullptr;
	Player3D* m_player3D = nullptr;
};

