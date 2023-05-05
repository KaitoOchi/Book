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
	m_position = { 100.0f, 100.0f, 0.0f };
	SetType(0);


	//監視カメラの設定
	m_modelRender.Init("Assets/modelData/object/SecurityCamera/camera_arm.tkm");
	m_modelRender.SetPosition(m_position);
	m_cameraModelRender.Init("Assets/modelData/object/SecurityCamera/camera_body.tkm");
	m_cameraModelRender.SetPosition(m_position);
	m_cameraModelRender.Update();

	//ライトセンサーの設定
	m_lightSensor = NewGO<LightSensor>(0, "lightSensor");
	m_lightSensor->SetState(LightSensor::enState_Rotate);
	m_lightSensor->SetLightNumber(m_lightNumber);
	m_lightSensor->SetPosition(m_position);
	m_lightSensor->SetRotation(m_rotation);
	m_lightSensor->SetDirection(m_scale);
	m_lightSensor->SetMaxTime(3.0f);
	m_lightSensor->SetMoveSpeed(Vector3(10.0f, 0.0f, 0.0f));

	Quaternion rot;
	rot = m_lightSensor->GetRotation();
	rot.AddRotationY(Math::DegToRad(-180));

	m_modelRender.SetRotation(rot);
	m_modelRender.Update();

	return true;
}

void SecurityCamera::Update()
{
	//カメラを回転させる
	Quaternion rot;
	rot = m_lightSensor->GetRotation();

	rot.AddRotationX(Math::DegToRad(30.0f + (m_type * 20.0f)));
	rot.AddRotationY(Math::DegToRad(-200.0f + (m_type * 380.0f)));
	rot.AddRotationZ(Math::DegToRad(30.0f + (m_type * -40.0f)));

	m_cameraModelRender.SetRotation(rot);
	m_cameraModelRender.Update();
}

void SecurityCamera::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_cameraModelRender.Draw(rc);
}