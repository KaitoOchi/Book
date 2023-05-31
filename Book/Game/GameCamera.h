#pragma once
class PlayerManagement;
class Player3D;
class Game;
class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
	
public:
	void SetPosition(const Vector3& m_pos)
	{
		m_target = m_pos;
	}

	//カメラの座標
	void SetCameraPositio(const Vector3& camerapos)
	{
		m_cameraposition = camerapos;
	}

	const Vector3 GetCameraPosition()
	{
		return m_cameraposition;
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
	Vector3 m_toCameraPos = Vector3::Zero;//注視点から視点に向かうベクトル
	Vector3 m_toCameraPosOld = Vector3::Zero;//注視点から視点に向かうベクトル変更前座標
	Vector3 m_target=Vector3::Zero;//注視点
	
	
	Vector3 axisX;//X軸周りの回転
	Vector3 m_cameraposition;
	Vector3 m_getPosition=Vector3::Zero;
	
	PlayerManagement* m_playerManagement = nullptr;
	Player3D* m_player3D = nullptr;
	Game* m_game = nullptr;

	float m_cameraMove = 0.0f;
	Vector3 m_cameraPosition = Vector3::Zero;

	Vector3 m_camePos = Vector3::Zero;
	Quaternion qRot;											//カメラの回転

	bool				m_pushState = true;
private:
	/// <summary>
	/// プレイヤーが捕まった時のカメラの動き
	/// </summary>
	void CatchMove();

	/// <summary>
	/// プレイヤーが向いている方向に回転させる
	/// </summary>
	void RotCamera();

};

