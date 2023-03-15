#pragma once
class BackGround:public IGameObject
{
public:
	BackGround();
	~BackGround();

	bool Start();
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標を設定する
	/// </summary>
	void SetPosition(Vector3 pos) {
		m_position = pos;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	void SetRotation(Quaternion rot) {
		m_rotation = rot;
	}

private:
	ModelRender m_ModelRender;			// モデルレンダー

	Quaternion m_rotation;					// 回転
	Vector3 m_position = Vector3::Zero;	// 座標
}