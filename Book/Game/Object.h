#pragma once
#include "Player3D.h"


class Object
{
public:
	Object();
	~Object();
	virtual void Update() = 0;

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
	
protected:
	/// <summary>
	/// �����蔻��̏����B
	/// </summary>
	void Collision();

	/// <summary>
	/// ���������Ƃ��̏����B
	/// </summary>
	virtual void Hit() = 0;

	ModelRender			m_modelRender;

	CollisionObject*	m_collisionObject = nullptr;
	Player3D*			m_player = nullptr;

	Vector3				m_position	= Vector3::Zero;
	Quaternion			m_rotation	= Quaternion::Identity;
	Vector3				m_scale		= Vector3::One;
};

