#pragma once

class PlayerManagement;

class Wall :public IGameObject
{
public:
	Wall();
	~Wall();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetModel(int num);
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
	/// �ǂ̃��f����ݒ肷��
	/// </summary>
	/// <param name="0">��{</param>
	/// <param name="1">�G�悠��</param>
	/// <param name = "2">���Ԃ���</param>
	/// <param name = "3">�X�^�[�g</param>
	/// <param name="4">�S�[��</param>
	void SetWallModel(int num) {
		ModelState = num;
	}

protected:
	PhysicsBoxObject m_physicsBoxObj;
	PhysicsGhostObject m_physicsGhostObj;
	ModelRender m_wallRender;							//�ǃ��f���̍쐬
	CollisionObject m_colliob;	

	PlayerManagement* m_player = nullptr;
	LevelRender m_levelRender;

	float posXmax = 0;
	float posXmin = 1000000.0;
	float posYmax = 0;
	float posYmin = 1000000.0;
	float posZmax = 0;
	float posZmin = 1000000.0;
	float posX;
	float posY;
	float posZ;

	Vector3 m_boxSize = Vector3::Zero;					//�{�b�N�X�R���C�_�[�̍쐬

	int ModelState = 0;									// ���f�����w�肷��
};

