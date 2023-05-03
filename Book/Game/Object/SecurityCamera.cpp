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
	m_lightSensor->SetMaxTime(3.0f);
	m_lightSensor->SetMoveSpeed(Vector3(10.0f, 0.0f, 0.0f));

	m_modelRender.Init("Assets/modelData/object/SecurityCamera/camera_arm.tkm");
	m_cameraModelRender.Init("Assets/modelData/object/SecurityCamera/camera_body.tkm");

	Vector3 pos = { -145.0f, 100.0f, 45.0f };
	m_position += pos;

	Quaternion rot;
	rot = m_lightSensor->GetRotation();
	rot.AddRotationY(Math::DegToRad(-180));

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(rot);
	m_modelRender.Update();
	m_cameraModelRender.SetPosition(m_position);
	m_cameraModelRender.Update();

	return true;
}

void SecurityCamera::Update()
{
	Quaternion rot;
	rot = m_lightSensor->GetRotation();
	rot.AddRotationX(Math::DegToRad(30.0f));
	rot.AddRotationY(Math::DegToRad(-200.0f));
	rot.AddRotationZ(Math::DegToRad(30.0f));

	m_cameraModelRender.SetRotation(rot);
	m_cameraModelRender.Update();
}

void SecurityCamera::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_cameraModelRender.Draw(rc);
}