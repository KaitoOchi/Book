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
	void SetPosition(Vector3 pos) {
		m_position = pos;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	void SetRotation(Quaternion rot) {
		m_rotation = rot;
	}

	/// <summary>
	/// �X�P�[����ݒ肷��
	/// </summary>
	void SetScale(Vector3 sca) {
		m_scale = sca;
	}

private:
	PhysicsStaticObject m_physicsStaticObj;
	ModelRender m_ModelRender;			// ���f�������_�[

	Quaternion m_rotation;				// ��]
	Vector3 m_position = Vector3::Zero;	// ���W
	Vector3 m_scale = Vector3::One;			// �X�P�[��
};