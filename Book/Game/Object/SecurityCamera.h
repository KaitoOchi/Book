#pragma once
#include "Object.h"
class LightSensor;

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
	/// ƒJƒƒ‰‚ÌŒü‚¢‚Ä‚¢‚é•ûŒü‚ğİ’è
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
	/// ”Ô†‚ğİ’èB
	/// </summary>
	void SetNumber(const int num);

private:
	void Hit() override
	{

	}

private:
	ModelRender		m_cameraModelRender;
	LightSensor*	m_lightSensor = nullptr;
	int				m_type = 0;
};