#pragma once

class PlayerManagement;
class Player3D;
class Game;

/// <summary>
/// �Q�[���J�����N���X�B
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
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="m_pos"></param>
	void SetPosition(const Vector3& m_pos)
	{
		m_target = m_pos;
	}

	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3 GetCameraPosition()
	{
		return m_cameraposition;
	}

	//�J�����̍��W
	void SetCameraPositio(const Vector3& camerapos)
	{
		m_cameraposition = camerapos;
	}

	/// <summary>
	/// �J�����̍��W
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
	/// �v���C���[���߂܂������̃J�����̓���
	/// </summary>
	void CatchMove();

	/// <summary>
	/// �v���C���[�������Ă�������ɉ�]������
	/// </summary>
	void RotCamera();

private:
	PlayerManagement*	m_playerManagement = nullptr;
	Player3D*			m_player3D = nullptr;
	Game*				m_game = nullptr;
	Vector3				m_toCameraPos;					//�����_���王�_�Ɍ������x�N�g��
	Vector3				m_toCameraPosOld;				//�����_���王�_�Ɍ������x�N�g���ύX�O���W
	Vector3				m_target;						//�����_
	Vector3				axisX;							//X������̉�]
	Vector3				m_cameraposition;
	Vector3				m_cameraPosition;
	Vector3				m_camePos;
	Quaternion			qRot;							//�J�����̉�]
	bool				m_pushState = true;
	float				m_cameraMove = 0.0f;
};

