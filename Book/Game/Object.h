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
	/// 座標を設定。
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// 回転を設定。
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// 回転を取得。
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// 表示状態を設定。
	/// </summary>
	void SetActive(const bool active)
	{
		m_isActive = active;
	}

	/// <summary>
	/// 表示状態を取得。
	/// </summary>
	bool GetActive()
	{
		return m_isActive;
	}
	
protected:
	/// <summary>
	/// 当たり判定の処理。
	/// </summary>
	void Collision();

	/// <summary>
	/// 当たったときの処理。
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

