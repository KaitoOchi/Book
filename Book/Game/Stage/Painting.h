#pragma once

namespace
{
	const int PAINTING_MAX = 15;	//絵画の最大数
}

/// <summary>
/// 絵画クラス。
/// </summary>
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
		int num = rand() % PAINTING_MAX;

		wchar_t path[255];
		swprintf_s(path, 255,
			L"Assets/sprite/painting/%d.DDS",
			num
		);

		m_texture.InitFromDDSFile(path);
	}

private:
	ModelRender m_modelRender;			//絵画モデル
	ModelRender m_frameModelRender;		//額縁モデル
	Texture		m_texture;				//絵画のテクスチャ
};