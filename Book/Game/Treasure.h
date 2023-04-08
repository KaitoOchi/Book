#pragma once
class Treasure :public IGameObject
{
public:
	Treasure();
	~Treasure();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//À•W‚Ìæ“¾
	const Vector3 GetPosition() const
	{
		return m_position;
	}
	void SetPosition(const Vector3 m_pos)
	{
		m_position = m_pos;
	}

	//‘å‚«‚³‚Ìæ“¾
	const Vector3 GetScale() const
	{
		return m_scale;
	}
	void SetScale(const Vector3 m_sca)
	{
		m_scale = m_sca;
	}

	/// <summary>
	/// ‰ñ“]‚Ìæ“¾
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()const
	{
		return m_rotation;
	}
	void SetRotation(const Quaternion m_rot)
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