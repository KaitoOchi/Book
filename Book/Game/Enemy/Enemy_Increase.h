#pragma once
class Game;
class Gage;
class Enemy_Increase :public IGameObject
{
public:
	Enemy_Increase();
	~Enemy_Increase();
	bool Start();
	void Update();
public:
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �|�W�V�����̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// �傫���̐ݒ�
	/// </summary>
	/// <param name="scl">�傫��</param>
	void SetScale(Vector3 scl)
	{
		m_scale = scl;
	}
	/// <summary>
	/// �傫���̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()
	{
		return m_scale;
	}
private:
	/// <summary>
	/// �G�l�~�[���N��������
	/// </summary>
	void Enemy_Open();

private:
	Vector3						m_position = Vector3::Zero;				//���W
	float						m_nearposition =FLT_MIN;
	Vector3						m_ifPosition=Vector3::Zero;
	Vector3						m_scale = Vector3::Zero;				//�傫��
	Quaternion					m_rotation;								//��]

	Game*						m_game = nullptr;						//�Q�[��
	Gage*						m_gage = nullptr;						//�Q�[�W
	PlayerManagement*			m_playerManagement = nullptr;			//�v���C���[�}�l�W�����g

	int m_increaseEnemy = 0;
	

};

