#pragma once
#include "Player.h"


class Object : public IGameObject
{
public:
	Object();
	~Object();
	virtual bool Start();
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

	/// <summary>
	/// �\����Ԃ�ݒ�B
	/// </summary>
	void SetActive(const bool active)
	{
		m_isActive = active;
	}

	/// <summary>
	/// �\����Ԃ��擾�B
	/// </summary>
	bool GetActive()
	{
		return m_isActive;
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
	Player*				m_player = nullptr;

	Vector3				m_position	= Vector3::Zero;
	Quaternion			m_rotation	= Quaternion::Identity;
	Vector3				m_scale		= Vector3::One;

	bool m_isActive = true;
};

