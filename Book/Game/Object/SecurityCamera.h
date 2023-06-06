#pragma once
#include "Object.h"

class LightSensor;

/// <summary>
/// �Ď��J�����N���X�B
/// </summary>
class SecurityCamera : public Object
{
public:
	SecurityCamera();
	~SecurityCamera();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// �J�����̌����Ă��������ݒ�
	/// </summary>
	void SetType(const int type)
	{
		switch (type)
		{
		case 0:
			m_rotation.SetRotationY(Math::DegToRad(0.0f));
			m_scale = Vector3(0.9f, -1.0f, 0.0f);
			break;

		case 1:
			m_rotation.SetRotationY(Math::DegToRad(90.0f));
			m_scale = Vector3(0.0f, -1.0f, -0.9f);
			break;

		case 2:
			m_rotation.SetRotationY(Math::DegToRad(180.0f));
			m_scale = Vector3(-0.9f, -1.0f, 0.0f);
			break;

		case 3:
			m_rotation.SetRotationY(Math::DegToRad(270.0f));
			m_scale = Vector3(0.0f, -1.0f, 0.9f);
			break;
		}

		m_type = type;
	}

	/// <summary>
	/// �ԍ���ݒ�B
	/// </summary>
	void SetNumber(const int num)
	{
		m_lightNumber = num;
	}

private:
	void Hit() override
	{

	}

private:
	ModelRender		m_cameraModelRender;		//�J�������f���B
	LightSensor*	m_lightSensor = nullptr;	//���C�g�Z���T�[�B
	int				m_lightNumber = 0;			//���C�g�̔ԍ��B
	int				m_type = 0;					//��ށB
};