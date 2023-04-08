#pragma once
class Treasure:public IGameObject
{
public:
	Treasure();
	~Treasure();
	bool Start();
	void Update();

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

private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	CollisionObject m_colliob;
};

