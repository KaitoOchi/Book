#pragma once
#include "Object.h"

class LightSensor : public Object
{
public:
	LightSensor();
	~LightSensor();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

protected:
	/// <summary>
	/// “–‚½‚Á‚½‚Æ‚«‚Ìˆ—B
	/// </summary>
	void Hit() override;

	SpotLight m_spotLight;

	float m_angle = 25.0f;

	FontRender m_fontRender;
};

