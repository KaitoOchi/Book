#pragma once
class BackGround :public IGameObject
{
public:
	BackGround();
	~BackGround();

	bool Start();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// �X�P�[����ݒ肷��
	/// </summary>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

private:
	PhysicsStaticObject		m_physicsStaticObj;
	ModelRender				m_modelRender;			// ���f�������_�[
	Vector3					m_position;				// ���W
	Vector3					m_scale = Vector3::One;	// �X�P�[��
	Quaternion				m_rotation;				// ��]
};