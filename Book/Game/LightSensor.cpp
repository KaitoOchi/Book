#include "stdafx.h"
#include "LightSensor.h"

#include "Player.h"


LightSensor::LightSensor()
{

}

LightSensor::~LightSensor()
{

}

bool LightSensor::Start()
{
	m_player = FindGO<Player>("player");

	m_position = Vector3(-80.0f, 50.0f, 0.0f);
	m_scale = Vector3(1.0f, 1.0f, 1.0f);

	m_isActive = false;

	m_spotLight.SetPosition(m_position);
	m_spotLight.SetColor(Vector3(50.0f, 0.0f, 0.0f));
	m_spotLight.SetRange(500.0f);
	m_spotLight.SetDirection(Vector3(1.0f, -1.0f, 1.0f));
	m_spotLight.SetAngle(m_angle);
	m_spotLight.SetCollisionObject(m_player->GetPosition());
	m_spotLight.Update();

	return true;
}

void LightSensor::Update()
{
	m_position.x += g_pad[0]->GetLStickXF();

	if (g_pad[0]->IsPress(enButtonB)) {
		m_position.y += g_pad[0]->GetLStickYF();
	}
	else {
		m_position.z += g_pad[0]->GetLStickYF();
	}

	//m_angle += g_pad[0]->GetRStickXF();

	m_spotLight.SetAngle(m_angle);
	m_spotLight.SetPosition(m_position);

	Quaternion qRotY;
	qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
	qRotY.Apply(m_spotLight.GetDirection());

	Vector3 rotAxis;
	rotAxis.Cross(g_vec3AxisY, m_spotLight.GetDirection());
	Quaternion qRotX;
	qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * 0.01f);

	qRotX.Apply(m_spotLight.GetDirection());

	Quaternion qRot;
	qRot.SetRotation({ 0.0f, 0.0f, -1.0f }, m_spotLight.GetDirection());

	m_spotLight.Update();

	wchar_t debugText[255];
	swprintf_s(debugText, 255, L"Angle:%.2f \nX:%.2f \nY:%.2f \nZ:%.2f",
		m_spotLight.GetAngle(), m_spotLight.GetDirection().x, m_spotLight.GetDirection().y, m_spotLight.GetDirection().z);
	m_fontRender.SetText(debugText);

	m_spotLight.HitRay();
}

void LightSensor::Hit()
{

}

void LightSensor::Render(RenderContext& rc)
{
	if (m_isActive)
		m_modelRender.Draw(rc);

	m_fontRender.Draw(rc);
}