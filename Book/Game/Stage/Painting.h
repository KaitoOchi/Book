#pragma once

class Painting : public IGameObject
{
public:
	Painting();
	~Painting();
	bool Start();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// 座標を設定。
	/// </summary>
	void SetPosition(const Vector3& pos)
	{
		Vector3 position = pos;
		position.y += 50.0f;

		m_modelRender.SetPosition(position);
		m_frameModelRender.SetPosition(position);
	}

	/// <summary>
	/// 回転を設定。
	/// </summary>
	void SetRotation(const Quaternion& rot)
	{
		Quaternion rotation;
		rotation.AddRotationDegZ(360.0f);
		rotation.Multiply(rot);

		m_modelRender.SetRotation(rotation);
		m_frameModelRender.SetRotation(rot);
	}

	/// <summary>
	/// 絵画の種類を設定。
	/// </summary>
	void SetType()
	{
		int num = rand() % 12;

		wchar_t path[255];
		swprintf_s(path, 255,
			L"Assets/sprite/painting/%d.DDS",
			num
		);

		m_texture.InitFromDDSFile(path);
	}

private:
	ModelRender m_modelRender;
	ModelRender m_frameModelRender;
	Texture m_texture;
};