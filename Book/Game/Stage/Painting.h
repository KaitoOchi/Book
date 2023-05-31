#pragma once

namespace
{
	const int PAINTING_MAX = 15;	//ŠG‰æ‚ÌÅ‘å”
}

class Painting : public IGameObject
{
public:
	Painting();
	~Painting();
	bool Start();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// À•W‚ğİ’èB
	/// </summary>
	void SetPosition(const Vector3& pos)
	{
		Vector3 position = pos;
		position.y += 50.0f;

		m_modelRender.SetPosition(position);
		m_frameModelRender.SetPosition(position);
	}

	/// <summary>
	/// ‰ñ“]‚ğİ’èB
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
	/// ŠG‰æ‚Ìí—Ş‚ğİ’èB
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
	ModelRender m_modelRender;
	ModelRender m_frameModelRender;
	Texture m_texture;
};