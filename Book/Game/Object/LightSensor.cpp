#include "stdafx.h"
#include "LightSensor.h"

#include "PlayerManagement.h"
#include "Gage.h"


LightSensor::LightSensor()
{

}

LightSensor::~LightSensor()
{

}

bool LightSensor::Start()
{
	m_player = FindGO<PlayerManagement>("playerManagement");
	m_gage = FindGO<Gage>("gage");
	m_isActive = false;

	//スポットライトを設定
	m_spotLight.SetSpotLight(
		1,
		m_position,
		Vector3(10.0f, 0.0f, 0.0f),
		500.0f,
		m_direction,
		m_angle
	);

	return true;
}

void LightSensor::Update()
{
	Time();

	switch (m_lightSensorState)
	{
	case enState_Move:
		Move();
		break;

	case enState_Rotate:
		Rotate();
		break;
	}

	//当たり判定
	if (m_spotLight.IsHit(m_player->GetPosition())) {
		m_gage->GageUp(2);
		
	}
}

void LightSensor::Time()
{
	//最大時間、最小時間を超えたら
	if (m_timer > m_maxTime) {
		m_isTimeOver = true;
	}
	else if (m_timer < -m_maxTime) {
		m_isTimeOver = false;
	}

	//タイムの増減
	if (m_isTimeOver) {
		m_timer -= g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_timer += g_gameTime->GetFrameDeltaTime();
	}
}

void LightSensor::Move()
{
	//スポットライトの位置を設定
	Vector3 pos = m_spotLight.GetPosition();
	pos += m_moveSpeed * (m_timer / 100.0f);

	m_spotLight.SetPosition(pos);
	m_spotLight.Update();
}

void LightSensor::Rotate()
{
	//Y軸周りの回転
	Quaternion qRotY;
	qRotY.SetRotationY(m_timer * 0.001f);
	qRotY.Apply(m_spotLight.GetDirection());

	//外積を求める
	Vector3 rotAxis;
	rotAxis.Cross(g_vec3AxisY, m_spotLight.GetDirection());

	Quaternion qRot;
	qRot.SetRotation({ 0.0f, 0.0f, -1.0f }, m_spotLight.GetDirection());

	m_spotLight.Update();
}