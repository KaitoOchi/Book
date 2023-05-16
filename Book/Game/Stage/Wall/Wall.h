#pragma once

class PlayerManagement;

class Wall :public IGameObject
{
public:
	Wall();
	virtual ~Wall()=0;
	bool Start();
	void WipeRender(RenderContext& rc);
	void Render(RenderContext& rc);

	void CreateBoxObj();		// �����蔻����쐬

	//���W�̎擾
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	void SetPosition(const Vector3& m_pos)
	{
		m_position = m_pos;
	}

	//�傫���̎擾
	const Vector3& GetScale()const
	{
		return m_scale;
	}
	void SetScale(const Vector3& m_scl)
	{
		m_scale = m_scl;
	}

	//��]�̎擾
	const Quaternion& GetRotation()const
	{
		return m_rotation;
	}
	void SetRotation(const Quaternion& m_rot)
	{
		m_rotation = m_rot;
	}
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation;

	/// <summary>
	/// �ǂƂ̋���
	/// </summary>
	/// <param name="pos">�v���C���[�̍��W</param>
	void SetWallRenderPosition(const Vector3 pos)
	{
		m_RenderPosition = pos;
	}



protected:
	PhysicsBoxObject m_physicsBoxObj;
	PhysicsGhostObject m_physicsGhostObj;
	ModelRender m_wallRender;							//�ǃ��f���̍쐬
	CollisionObject m_colliob;	

	PlayerManagement* m_player = nullptr;
	LevelRender m_levelRender;

	Vector3 m_boxSize = Vector3::Zero;					//�{�b�N�X�R���C�_�[�̍쐬

	Vector3 m_RenderPosition = Vector3::Zero;
};

