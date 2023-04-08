#pragma once
class Treasure :public IGameObject
{
public:
	Treasure();
	~Treasure();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//座標の取得
	const Vector3 GetPosition() const
	{
		return m_position;
	}
	void SetPosition(const Vector3 m_pos)
	{
		m_position = m_pos;
	}

	//大きさの取得
	const Vector3 GetScale() const
	{
		return m_scale;
	}
	void SetScale(const Vector3 m_sca)
	{
		m_scale = m_sca;
	}

	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()const
	{
		return m_rotation;
	}
	void SetScale(const Quaternion m_rot)
	{
		m_rotation = m_rot;
	}
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation;
	CollisionObject m_colliob;
	ModelRender m_modelRender;
};