#pragma once
class BackGround:public IGameObject
{
public:
	BackGround();
	~BackGround();

	bool Start();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	void SetPosition(Vector3 pos) {
		position = pos;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	void SetRotation(Quaternion rot) {
		rotation = rot;
	}

private:
	ModelRender m_ModelRender;			// ���f�������_�[

	Quaternion rotation;					// ��]
	Vector3 position = Vector3::Zero;	// ���W
}