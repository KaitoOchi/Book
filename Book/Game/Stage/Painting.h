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
	/// ���W��ݒ�B
	/// </summary>
	void SetPosition(const Vector3& pos)
	{
		m_modelRender.SetPosition(pos);
		m_frameModelRender.SetPosition(pos);
	}

	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	void SetRotation(const Quaternion& rot)
	{
		m_modelRender.SetRotation(rot);
		m_frameModelRender.SetRotation(rot);
	}

	/// <summary>
	/// �G��̎�ނ�ݒ�B
	/// </summary>
	void SetType()
	{
		int num = rand() % 4;

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