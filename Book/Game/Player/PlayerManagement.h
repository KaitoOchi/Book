#pragma once
#include "Player2D.h"
#include "Player3D.h"

class Player2D;
class Player3D;
class GameCamera;
class Game;

/// <summary>
/// プレイヤーマネジメントクラス。
/// </summary>
class PlayerManagement:public IGameObject
{
public:
	/// <summary>
	/// プレイヤー状態の列挙型
	/// </summary>
	enum EnManagementState
	{
		m_enPlayer_GhostHit,	//透明なブロックに当たっている間
		m_enPlayer_Changing,	//切替中
		m_enPlayer_Stop,		//停止中
		m_enPlayer_2DChanging,	//2Dプレイヤー
		m_enPlayer_3DChanging,	//3Dプレイヤー
	};
	EnManagementState m_enMananagementState = m_enPlayer_3DChanging;

public:
	PlayerManagement();
	~PlayerManagement();
	bool Start();
	void Update();

public:
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

	/// <summary>
	/// 移動速度を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3 GetMoveSpeed()
	{
		// 今アクティブなプレイヤーの速度を返す
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
	/// エフェクトの取得。
	/// </summary>
	EffectEmitter* GetEffect()
	{
		return m_smokeEffect;
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

	/// <summary>
	/// ステートを取得。
	/// </summary>
	/// <returns></returns>
	const Player::EnPlayerState& GetEnPlayerState()
	{
		// 今アクティブなプレイヤーのステート
		if (m_enMananagementState == m_enPlayer_3DChanging) {
			return m_player3D->GetPlayerState();
		}
		else {
			return m_player2D->GetPlayerState();
		}
	}

	/// <summary>
	/// スタミナを取得。
	/// </summary>
	/// <returns></returns>
	const float GetStamina()const
	{
		// 今アクティブなプレイヤーのスタミナ量を返す
		if (m_enMananagementState == m_enPlayer_3DChanging) {
			return m_player3D->GetStamina();
		}
		else if (m_enMananagementState == m_enPlayer_2DChanging) {
			return m_player2D->GetStamina();
		}
		else if (m_manageStateTmp == m_enPlayer_3DChanging)
		{
			return m_player2D->GetStamina();
		}
		else if (m_manageStateTmp == m_enPlayer_2DChanging)
		{
			return m_player3D->GetStamina();
		}
	}

	/// <summary>
	/// 走れるかどうかの取得。
	/// </summary>
	/// <returns></returns>
	const bool GetRunState()
	{
		// 今アクティブなプレイヤーの走れるかどうかの判定
		if (m_enMananagementState == m_enPlayer_3DChanging) {
			return m_player3D->GetRunState();
		}
		else if (m_enMananagementState == m_enPlayer_2DChanging) {
			return m_player2D->GetRunState();
		}
		else if (m_manageStateTmp == m_enPlayer_3DChanging)
		{
			return m_player2D->GetRunState();
		}
		else if (m_manageStateTmp == m_enPlayer_2DChanging)
		{
			return m_player3D->GetRunState();
		}
	}

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
	CharacterController*	m_characon = nullptr;						//キャラクターコントローラー
	Player2D*				m_player2D = nullptr;						//プレイヤー2D
	Player3D*				m_player3D = nullptr;						//プレイヤー3D
	GameCamera*				m_gamecamera = nullptr;						//ゲームカメラ
	Game*					m_game = nullptr;							//ゲームクラス。
	EffectEmitter*			m_smokeEffect = nullptr;					//煙エフェクト
	EnManagementState		m_manageStateTmp = m_enPlayer_3DChanging;	//遷移するステートの一時変数
	bool					m_GameStartState = false;					//ゲームが始まっているかどうか
	float					m_changeTime = 0.0f;						//プレイヤーを切り替える時間
};

