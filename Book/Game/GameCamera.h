#pragma once
class PlayerManagement;
class Player3D;
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

	//�J�����̍��W
	void SetCameraPositio(const Vector3& camerapos)
	{
		m_cameraposition = camerapos;
	}

	const Vector3 GetCameraPosition()
	{
		return m_cameraposition;
	}

private:
	Vector3 m_toCameraPos = Vector3::Zero;//�����_���王�_�Ɍ������x�N�g��
	Vector3 m_toCameraPosOld = Vector3::Zero;//�����_���王�_�Ɍ������x�N�g���ύX�O���W
	Vector3 m_target=Vector3::Zero;//�����_
	
	Quaternion qRot;//Y������̉�]
	
	Vector3 axisX;//X������̉�]
	Vector3 m_cameraposition;
	Vector3 m_getPosition=Vector3::Zero;
	
	PlayerManagement* m_playerManagement = nullptr;
	Player3D* m_player3D = nullptr;

	float m_cameraMove = 0.0f;
	Vector3 m_cameraPosition = Vector3::Zero;

private:
	/// <summary>
	/// �v���C���[���߂܂������̃J�����̓���
	/// </summary>
	void CatchMove();
};

