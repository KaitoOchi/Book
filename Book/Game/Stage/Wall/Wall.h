#pragma once

class PlayerManagement;

/// <summary>
/// �ǂ̊��N���X�B
/// </summary>
class Wall :public IGameObject
{
public:
	Wall();
	virtual ~Wall();
	bool Start();
	void WipeRender(RenderContext& rc);
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// �����蔻����쐬�B
	/// </summary>
	void CreateBoxObj();

	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="m_pos"></param>
	void SetPosition(const Vector3& m_pos)
	{
		m_position = m_pos;
	}

	/// <summary>
	/// ���W�̎擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// �g�嗦��ݒ�B
	/// </summary>
	/// <returns></returns>
	void SetScale(const Vector3& m_scl)
	{
		m_scale = m_scl;
	}

	/// <summary>
	/// �g�嗦���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <returns></returns>
	void SetRotation(const Quaternion& m_rot)
	{
		m_rotation = m_rot;
	}

	/// <summary>
	/// ��]�̎擾�B
	/// </summary>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/// <summary>
	/// �ǂƂ̋���
	/// </summary>
	/// <param name="pos">�v���C���[�̍��W</param>
	void SetWallRenderPosition(const Vector3 pos)
	{
		m_playerPos= pos;
	}



protected:
	PhysicsBoxObject					m_physicsBoxObj;
	PhysicsGhostObject					m_physicsGhostObj;
	ModelRender							m_wallRender;							//�ǃ��f���̍쐬
	CollisionObject						m_colliob;	
	PlayerManagement*					m_player = nullptr;
	Quaternion							m_rotation;								//��]
	Vector3								m_position;								//���W
	Vector3								m_scale = Vector3::One;					//�g�嗦
	Vector3								m_playerPos;							//�v���C���[�̍��W
};

