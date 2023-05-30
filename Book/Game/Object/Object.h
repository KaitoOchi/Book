#pragma once
#include "PlayerManagement.h"
#include "Game.h";
class Gage;
class Player3D;

class Object : public IGameObject
{
public:
	Object();
	virtual ~Object();
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
	/// 大きさを設定。
	/// </summary>
	/// <param name="rot"></param>
	void SetScale(const Vector3& scl)
	{
		m_scale = scl;
	}

	/// <summary>
	/// 大きさを取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale()
	{
		return m_scale;
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

	enum EnHitState {
		m_enObject_null,		//当たっている
		m_enObject_hit,			//当たっていない
	};


	EnHitState m_HitState = m_enObject_null;

    CollisionObject* GetCollision()
	{
		return m_collisionObject;
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
	PlayerManagement*	m_player = nullptr;
	Game*				m_game = nullptr;
	Player3D*			m_player3d = nullptr;
	Vector3				m_position	= Vector3::Zero;
	Quaternion			m_rotation	= Quaternion::Identity;
	Vector3				m_scale		= Vector3::One;

	bool m_isActive = true;
};

