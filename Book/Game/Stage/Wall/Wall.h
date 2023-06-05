#pragma once

class PlayerManagement;

/// <summary>
/// 壁の基底クラス。
/// </summary>
class Wall :public IGameObject
{
public:
	Wall();
	virtual ~Wall();
	bool Start();
	void WipeRender(RenderContext& rc);
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// 当たり判定を作成。
	/// </summary>
	void CreateBoxObj();

	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="m_pos"></param>
	void SetPosition(const Vector3& m_pos)
	{
		m_position = m_pos;
	}

	/// <summary>
	/// 座標の取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// 拡大率を設定。
	/// </summary>
	/// <returns></returns>
	void SetScale(const Vector3& m_scl)
	{
		m_scale = m_scl;
	}

	/// <summary>
	/// 拡大率を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/// <summary>
	/// 回転を設定。
	/// </summary>
	/// <returns></returns>
	void SetRotation(const Quaternion& m_rot)
	{
		m_rotation = m_rot;
	}

	/// <summary>
	/// 回転の取得。
	/// </summary>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/// <summary>
	/// 壁との距離
	/// </summary>
	/// <param name="pos">プレイヤーの座標</param>
	void SetWallRenderPosition(const Vector3 pos)
	{
		m_playerPos= pos;
	}



protected:
	PhysicsBoxObject					m_physicsBoxObj;
	PhysicsGhostObject					m_physicsGhostObj;
	ModelRender							m_wallRender;							//壁モデルの作成
	CollisionObject						m_colliob;	
	PlayerManagement*					m_player = nullptr;
	Quaternion							m_rotation;								//回転
	Vector3								m_position;								//座標
	Vector3								m_scale = Vector3::One;					//拡大率
	Vector3								m_playerPos;							//プレイヤーの座標
};

