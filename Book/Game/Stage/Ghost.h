#pragma once
#include "physics/PhysicsGhostObject.h"

class Ghost : public IGameObject
{
public:
	Ghost();
	virtual ~Ghost();
	bool Start();
	void Update();

	/// <summary>
	/// �ڐG����
	/// </summary>
	bool IsHit(const Vector3& playerPosition);

private:
	/// <summary>
	/// �������v�Z����
	/// </summary>
	/// <param name="playerPosition"></param>
	/// <returns></returns>
	Vector3 RangeWithTarget(const Vector3& playerPosition);

public:
	/// <summary>
	/// �ǂ̃I�u�W�F�N�g���쐬���邩
	/// </summary>
	enum EnGhostObject
	{
		m_enGhostObje_physicsGhost,		//�S�[�X�g�I�u�W�F�N�g
		m_enGhostObje_boxCollider,		//�{�b�N�X�R���C�_�[
	};

public:
	/// <summary>
	/// �{�b�N�X�̃T�C�Y�����߂�B
	/// </summary>
	Vector3& CreateGhostBox();
	
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// ��]���擾�B
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// �傫����ݒ�B
	/// </summary>
	/// <param name="rot"></param>
	void SetScale(const Vector3& scl)
	{
		m_scale = scl;
	}

	/// <summary>
	/// �傫�����擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale()
	{
		return m_scale;
	}
	
protected:
	ModelRender m_modelRender;
	Vector3		m_position = Vector3::Zero;
	Vector3		m_scale = Vector3::One;
	Quaternion	m_rotation = Quaternion::Identity;
};

