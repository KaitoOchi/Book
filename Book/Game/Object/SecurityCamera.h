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

private:
	void Hit() override
	{

	}

private:
	ModelRender m_cameraModelRender;

	LightSensor* m_lightSensor = nullptr;

	float m_a = 0.0f;
	float m_b = 0.0f;
};