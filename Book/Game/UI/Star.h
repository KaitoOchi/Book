#pragma once
#include "PlayerManagement.h"
class Star:public IGameObject
{
public:
	Star();
	~Star();
	bool Start();
	void Update();
	void Rotaition();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="m_pos"></param>
	void SetPosition(const Vector3 m_pos)
	{
		m_position = m_pos;
	}

	// �O������X�V������
	void ModelRenderUpdate() {
		m_starRender.SetPosition(m_position);
		m_starRender.Update();
	}

private:


	ModelRender m_starRender;
	Quaternion m_rot;
	Vector3 m_position = Vector3::Zero;
};

