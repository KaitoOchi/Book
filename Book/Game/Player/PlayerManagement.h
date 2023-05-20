#pragma once
#include "Player2D.h"
#include "Player3D.h"
class Player2D;
class Player3D;
class PhysicsGhost;
class GameCamera;
class Game;
class PlayerManagement:public IGameObject
{
public:
	/// <summary>
	/// プレイヤー状態の列挙型
	/// </summary>
	enum EnManagementState
	{
		m_enPlayer_GhostHit,  //透明なブロックに当たっている間
		m_enPlayer_Changing,  //切替中
		m_enPlayer_Stop,  //停止中
		m_enPlayer_2DChanging,//2Dプレイヤー
		m_enPlayer_3DChanging,//3Dプレイヤー
	};
	EnManagementState m_enMananagementState = m_enPlayer_3DChanging;//３D状態
	float m_startTime = 2.0f;									//ゲームが始まるまでの時間
public:
	PlayerManagement();
	~PlayerManagement();
	bool Start();
	void Update();

public:
	/// <summary>
	/// 座標の設定。
	/// </summary>
	/// <param name="m_pos"></param>
	void  SetPosition(const Vector3& m_pos)
	{
		m_position = m_pos;
	}

	/// <summary>
	/// 座標の取得。
	/// </summary>
	const Vector3& GetPosition()const
	{
		// 今アクティブなプレイヤーの座標を返す
		if (m_enMananagementState == m_enPlayer_3DChanging) {
			return m_player3D->GetPosition();
		}
		else if (m_enMananagementState == m_enPlayer_2DChanging) {
			return m_player2D->GetPosition();
		}
		else if (m_manageStateTmp == m_enPlayer_3DChanging)
		{
			return m_player2D->GetPosition();
		}
		else if (m_manageStateTmp == m_enPlayer_2DChanging)
		{
			return m_player3D->GetPosition();
		}
	}

	const Vector3 GetMoveSpeed()
	{
		// 今アクティブなプレイヤーの座標を返す
		if (m_enMananagementState == m_enPlayer_3DChanging) {
			return m_player3D->GetMoveSpeed();
		}
		else if (m_enMananagementState == m_enPlayer_2DChanging) {
			return m_player2D->GetMoveSpeed();
		}
		else if (m_manageStateTmp == m_enPlayer_3DChanging)
		{
			return m_player2D->GetMoveSpeed();
		}
		else if (m_manageStateTmp == m_enPlayer_2DChanging)
		{
			return m_player3D->GetMoveSpeed();
		}
	}

	/// <summary>
	/// キャラコンの設定
	/// </summary>
	/// <param name="m_chara"></param>
	void SetCharacon(CharacterController* m_chara)
	{
		m_characon = m_chara;
	}

	/// <summary>
	/// キャラコンの取得。
	/// </summary>
	/// <returns></returns>
	CharacterController* GetCharacon()
	{
		return m_characon;
	}

	/// <summary>
	/// 3Dプレイヤーの取得。
	/// </summary>
	/// <returns></returns>
	Player3D* GetPlayer3D()
	{
		return m_player3D;
	}

	/// <summary>
	/// 2Dプレイヤーの取得。
	/// </summary>
	/// <returns></returns>
	Player2D* GetPlayer2D()
	{
		return m_player2D;
	}

	/// <summary>
	/// ゲーム開始前のステートを設定。
	/// </summary>
	void SetGameState(const bool state)
	{
		m_GameStartState = state;
	}

	/// <summary>
	/// ゲーム開始前のステートを取得。
	/// </summary>
	const bool GetGameState()
	{
		return m_GameStartState;
	}

	/// <summary>
	/// 外部から切替を行う
	/// </summary>
	void SetChange(EnManagementState manaState);

	const Player::EnPlayerState& GetEnPlayerState()
	{
		// 今アクティブなプレイヤーの座標を返す
		if (m_enMananagementState == m_enPlayer_3DChanging) {
			return m_player3D->GetPlayerState();
		}
		else {
			return m_player2D->GetPlayerState();
		}
	}

	const float GetStamina()const
	{
		// 今アクティブなプレイヤーの座標を返す
		if (m_enMananagementState == m_enPlayer_3DChanging) {
			return m_player3D->GetStamina();
		}
		else {
			return m_player2D->GetStamina();
		}
	}

private:
	
	bool RestartState = true;									//プレイヤーを再開させるかどうか
	bool ChangeStart = true;

private:
	/// <summary>
	/// 入力処理。
	/// </summary>
	void Input();

	/// <summary>
	/// プレイヤーを2Dに変更する。
	/// </summary>
	void PlayerChange2D();

	/// <summary>
	/// プレイヤーを3Dに変更する。
	/// </summary>
	void PlayerChange3D();

	/// <summary>
	/// 切り替え中かどうか。
	/// </summary>
	void IsChanging();

private:
	
	CharacterController*	m_characon = nullptr;
	Player2D*				m_player2D = nullptr;
	Player3D*				m_player3D = nullptr;
	GameCamera*				m_gamecamera = nullptr;
	PhysicsGhost*			m_physicsghost = nullptr;
	Game*					m_game = nullptr;
	Vector3					m_ghostPosition = Vector3::Zero;
	Vector3					m_position = Vector3::Zero;
	EnManagementState		m_manageStateTmp = m_enPlayer_3DChanging;		//遷移するステートの一時変数
	bool					m_GameStartState = false;						//ゲームが始まっているかどうか
	float					m_changeTime = 0.0f;							//プレイヤーを切り替える時間

	EffectEmitter* m_smokeEffect = nullptr;

};

