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
		position = pos;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	void SetRotation(Quaternion rot) {
		rotation = rot;
	}

private:
	ModelRender m_ModelRender;			// モデルレンダー

	Quaternion rotation;					// 回転
	Vector3 position = Vector3::Zero;	// 座標
}