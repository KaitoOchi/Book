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
	Vector3 m_toCameraPos = Vector3::Zero;//�����_���王�_�Ɍ������x�N�g��
	Vector3 m_toCameraPosOld = Vector3::Zero;//�����_���王�_�Ɍ������x�N�g���ύX�O���W
	Vector3 m_target=Vector3::Zero;//�����_
	Quaternion qRot;//Y������̉�]
	Vector3 axisX;//X������̉�]

	PlayerManagement* m_playerManagement = nullptr;
};

