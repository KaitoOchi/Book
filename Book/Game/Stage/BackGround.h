#pragma once
class BackGround :public IGameObject
{
public:
	BackGround();
	~BackGround();

	bool Start();
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標を設定する
	/// </summary>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// スケールを設定する
	/// </summary>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

private:
	PhysicsStaticObject		m_physicsStaticObj;
	ModelRender				m_modelRender;			// モデルレンダー
	Vector3					m_position;				// 座標
	Vector3					m_scale = Vector3::One;	// スケール
	Quaternion				m_rotation;				// 回転
};