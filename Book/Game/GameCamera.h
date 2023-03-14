#pragma once
class PlayerManagement;
class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
	void SetPosition(const Vector3& m_pos)
	{
		m_target = m_pos;
	}
private:
	Vector3 m_toCameraPos = Vector3::Zero;//注視点から視点に向かうベクトル
	Vector3 m_toCameraPosOld = Vector3::Zero;//注視点から視点に向かうベクトル変更前座標
	Vector3 m_target=Vector3::Zero;//注視点
	Quaternion qRot;//Y軸周りの回転
	Vector3 axisX;//X軸周りの回転

	PlayerManagement* m_playerManagement = nullptr;
};

