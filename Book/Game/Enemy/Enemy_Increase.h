#pragma once
class Game;
class Enemy_Increase :public IGameObject
{
public:
	Enemy_Increase();
	~Enemy_Increase();
	bool Start();
	void Update();
	void NewEnemy(int number);
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


private:
	Vector3 m_position = Vector3::Zero;				//���W
	Vector3 m_scale = Vector3::Zero;				//�傫��
	Quaternion m_rotation;							//��]

	Game* m_game = nullptr;
};

