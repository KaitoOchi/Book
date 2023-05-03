#pragma once
#include "Object.h"
class Gage;

class LightSensor : public Object
{
public:
	//ライトセンサーの移動状態
	enum LightSensorState
	{
		enState_Move,
		enState_Rotate,
	};

public:
	LightSensor();
	~LightSensor();
	bool Start() override;
	void Update() override;

public:
	
	/// <summary>
	/// ステートを設定。
	/// </summary>
	void SetState(const LightSensorState& state)
	{
		m_lightSensorState = state;
	}

	/// <summary>
	/// 方向を設定。
	/// </summary>
	void SetDirection(const Vector3& dir)
	{
		m_direction = dir;
	}

	/// <summary>
	/// 移動速度を設定。
	/// </summary>
	void SetMoveSpeed(const Vector3& movSpd)
	{
		m_moveSpeed = movSpd;
	}

	/// <summary>
	/// 放射角度を設定。
	/// </summary>
	void SetAngle(const float angle)
	{
		m_angle = angle;
	}

	/// <summary>
	/// 最大時間を設定。
	/// </summary>
	void SetMaxTime(const float time)
	{
		m_maxTime = time;
	}

private:
	/// <summary>
	/// タイムの計測処理。
	/// </summary>
	void Time();

	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();

	/// <summary>
	/// 回転処理。
	/// </summary>
	void Rotate();

	void Hit() override
	{

	}

private:
	SpotLight			m_spotLight;			//スポットライト
	Gage*				m_gage = nullptr;		//ゲームUI
	LightSensorState	m_lightSensorState = enState_Move;
	Vector3				m_direction;			//方向
	Vector3				m_moveSpeed;			//移動速度
	bool				m_isTimeOver = false;	//タイムを超えたか
	float				m_angle = 25.0f;		//放射角度
	float				m_timer = 0.0f;			//時間
	float				m_maxTime = 5.0f;		//最大時間
};

