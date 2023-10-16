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
	/// 接触判定
	/// </summary>
	bool IsHit(const Vector3& playerPosition);

private:
	/// <summary>
	/// 距離を計算する
	/// </summary>
	/// <param name="playerPosition"></param>
	/// <returns></returns>
	Vector3 RangeWithTarget(const Vector3& playerPosition);

public:
	/// <summary>
	/// どのオブジェクトを作成するか
	/// </summary>
	enum EnGhostObject
	{
		m_enGhostObje_physicsGhost,		//ゴーストオブジェクト
		m_enGhostObje_boxCollider,		//ボックスコライダー
	};

public:
	/// <summary>
	/// ボックスのサイズを求める。
	/// </summary>
	Vector3& CreateGhostBox();
	
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
	
protected:
	ModelRender m_modelRender;
	Vector3		m_position = Vector3::Zero;
	Vector3		m_scale = Vector3::One;
	Quaternion	m_rotation = Quaternion::Identity;
};

