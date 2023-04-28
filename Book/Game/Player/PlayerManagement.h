﻿#pragma once
#include "Player2D.h"
#include "Player3D.h"
class Player2D;
class Player3D;
class Ghost;
class PlayerManagement:public IGameObject
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
	void SetPlayer2DAND3D(Player3D* player3d,Player2D* player2d)
	{
		m_player2D = player2d;
		m_player3D = player3d;
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
		m_enPlayer_GhostHit,  //透明なブロックに当たっている間
		m_enPlayer_Changing,  //切替中
		m_enPlayer_2DChanging,//2Dプレイヤー
		m_enPlayer_3DChanging,//3Dプレイヤー
	};
	EnMnagementState m_enMnanagementState = m_enPlayer_3DChanging;//３D状態

	bool m_GameStartState = false;								//ゲームが始まっているかどうか

	void Changing();

private:
	Vector3 m_ghostPosition=Vector3::Zero;
	
	Vector3 m_position = Vector3::Zero;
	CharacterController* m_characon = nullptr;
	Player2D* m_player2D = nullptr;
	Player3D* m_player3D = nullptr;
	Ghost* m_ghost = nullptr;
	float YLstickamount = 0;

	float m_startTime = 2.0f;									//ゲームが始まるまでの時間

	float m_changeTime = 5.0f;									//プレイヤーを切り替える時間
};

