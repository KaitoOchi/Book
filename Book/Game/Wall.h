#pragma once
class Wall :public IGameObject
{
public:
	Wall();
	~Wall();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
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
private:
	PhysicsStaticObject m_physicsStaticObj;
	
	ModelRender m_wallRender;					//�ǃ��f���̍쐬
	CollisionObject m_colliob;					//�{�b�N�X�R���C�_�[�̍쐬
};

