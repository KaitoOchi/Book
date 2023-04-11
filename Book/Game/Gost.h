#pragma once
#include "physics/PhysicsGhostObject.h"

class Gost:public IGameObject
{
public:
	Gost();
	~Gost();
	bool Start();
	void Update();
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
	PhysicsGhostObject m_physicsGhostObj;
private:
	

	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation;
	float posXmax = 0;
	float posXmin = 1000000.0;
	float posYmax = 0;
	float posYmin = 1000000.0;
	float posZmax = 0;
	float posZmin = 1000000.0;
	float posX;
	float posY;
	float posZ;
	Vector3 m_boxSize = Vector3::Zero;
	ModelRender m_modelRender;
};

