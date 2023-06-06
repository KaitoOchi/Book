#pragma once

class PlayerManagement;
class Player3D;
class Game;

/// <summary>
/// ゲームカメラクラス。
/// </summary>
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
	
public:
	/// <summary>
	/// 注視点を設定。
	/// </summary>
	/// <param name="m_pos"></param>
	void SetTarget(const Vector3& target)
	{
		m_target = target;
	}

	/// <summary>
	/// プレイヤーが切替中の座標を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3 GetCameraPosition()
	{
		return m_playerChangeCameraPosition;
	}

	//プレイヤーが切替中の座標
	void SetCameraPositio(const Vector3& camerapos)
	{
		m_playerChangeCameraPosition = camerapos;
	}

	/// <summary>
	/// カメラの座標
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition()
	{
		return m_camePos;
	}

	void SetPushState(bool state)
	{
		m_pushState = state;
	}

private:
	/// <summary>
	/// プレイヤーが捕まった時のカメラの動き
	/// </summary>
	void CatchMove();

	/// <summary>
	/// プレイヤーが向いている方向に回転させる
	/// </summary>
	void RotCamera();

private:
	PlayerManagement*	m_playerManagement = nullptr;
	Player3D*			m_player3D = nullptr;
	Game*				m_game = nullptr;
	Vector3				m_toCameraPos;					//注視点から視点に向かうベクトル
	Vector3				m_toCameraPosOld;				//注視点から視点に向かうベクトル変更前座標
	Vector3				m_target;						//注視点
	Vector3				axisX;							//X軸周りの回転
	Vector3				m_playerChangeCameraPosition;	//プレイヤーが切替中のカメラ座標
	Vector3				m_caughtCameraPosition;			//プレイヤーが捕まった時の座標
	Vector3				m_camePos;						//カメラ座標
	Quaternion			qRot;							//カメラの回転
	bool				m_pushState = true;
	float				m_cameraMove = 0.0f;
};

