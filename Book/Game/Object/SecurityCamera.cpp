#include "stdafx.h"
#include "SecurityCamera.h"

#include "LightSensor.h"


SecurityCamera::SecurityCamera()
{

}

SecurityCamera::~SecurityCamera()
{
	DeleteGO(m_lightSensor);
}

bool SecurityCamera::Start()
{
	m_lightSensor = NewGO<LightSensor>(0, "lightSensor");
	m_lightSensor->SetState(LightSensor::enState_Rotate);
	m_lightSensor->SetPosition(Vector3(-80.0f, 100.0f, 0.0f));
	m_lightSensor->SetDirection(Vector3(0.75f, -1.0f, 0.0f));
	m_lightSensor->SetMaxTime(5.0f);
	m_lightSensor->SetMoveSpeed(Vector3(10.0f, 0.0f, 0.0f));

	m_modelRender.Init("Assets/modelData/object/SecurityCamera/camera_arm.tkm");
	m_cameraModelRender.Init("Assets/modelData/object/SecurityCamera/camera_body.tkm");

	Vector3 pos = { -145.0f, 100.0f, 45.0f };

	m_position += pos;

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	m_cameraModelRender.SetPosition(m_position);
	m_cameraModelRender.Update();

	return true;
}

void SecurityCamera::Update()
{
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_position.z -= 1.0f;
	}
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_position.z += 1.0f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_position.x -= 1.0f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_position.x += 1.0f;
	}

	if (g_pad[0]->IsPress(enButtonA)) {
		m_position.x = m_position.x;
	}

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_lightSensor->GetRotation());
	m_modelRender.Update();
	m_cameraModelRender.SetPosition(m_position);
	m_cameraModelRender.SetRotation(m_lightSensor->GetRotation());
	m_cameraModelRender.Update();
}

void SecurityCamera::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_cameraModelRender.Draw(rc);
}