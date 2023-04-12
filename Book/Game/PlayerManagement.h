#pragma once
#include "Player.h"
#include "Player2D.h"
#include "Player3D.h"
class Player2D;
class Player3D;
class Ghost;
class PlayerManagement:public Player
{
public:
	PlayerManagement();
	~PlayerManagement();
	bool Start();
	void Update()override;
	void PlayerChange();
	void PlayerChange2D();
	void PlayerChange3D();
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


	//キャラコンの取得
	void SetCharacon(CharacterController* m_chara)
	{
		m_characon = m_chara;
	}
	CharacterController* GetCharacon()
	{
		return m_characon;
	}
	
	
	/// <summary>
	/// 共通のステート遷移処理
	/// </summary>
	void ProcessCommonStateTransition();
	enum EnMnagementState
	{
		m_enPlayer_2DChanging,//2Dに切替中
		m_enPlayer_3DChanging,//3Dに切替中
	};
	EnMnagementState m_enMnanagementState = m_enPlayer_3DChanging;//３D状態

	/// <summary>
	/// 透明ブロックの当たり判定
	/// </summary>
	void GhostHit();

	std::vector<Vector3> m_ghostpositions;
private:
	Vector3 m_ghostPosition=Vector3::Zero;
	Vector3 m_keepGhostPosition=Vector3::Zero;
	Vector3 m_position = Vector3::Zero;
	CharacterController* m_setChara = nullptr;
	Player2D* m_player2D = nullptr;
	Player3D* m_player3D = nullptr;
	
	Ghost* m_ghost = nullptr;
};

