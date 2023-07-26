#include "stdafx.h"
#include "LightSensor.h"

#include "PlayerManagement.h"
#include "Gage.h"

namespace
{
	const Vector3	ROTATION_Z_FROM = Vector3(0.0f, 0.0f, -1.0f);	//回転の元値。
	const int		VIGIRANCE_GAUGE_UP = 2;							//警戒度ゲージの上昇量。
	const float		ROTATION_SPEED = 0.0025f;						//回転速度。
	const float		PERCENT_MAX = 100.0f;							//割合の最大値。
}


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
		m_number,
		m_position,
		Vector3(1.5f, 0.1f, 0.0f),
		400.0f,
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
		m_gage->GageUp(VIGIRANCE_GAUGE_UP, false);
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
	pos += m_moveSpeed * (m_timer / PERCENT_MAX);

	m_spotLight.SetPosition(pos);
	m_spotLight.Update();
}

void LightSensor::Rotate()
{
	//Y軸周りの回転
	Quaternion qRotY;
	qRotY.SetRotationY(m_timer * ROTATION_SPEED);
	Vector3 spotLightDirection = m_spotLight.GetDirection();
	qRotY.Apply(spotLightDirection);
	m_spotLight.SetDirection(spotLightDirection);

	//外積を求める
	Vector3 rotAxis;
	rotAxis.Cross(g_vec3AxisY, spotLightDirection);

	m_rotation.SetRotation(ROTATION_Z_FROM, spotLightDirection);

	m_spotLight.Update();
}