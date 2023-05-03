#pragma once
#include "Object.h"
class Gage;

class LightSensor : public Object
{
public:
	//���C�g�Z���T�[�̈ړ����
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
	/// �X�e�[�g��ݒ�B
	/// </summary>
	void SetState(const LightSensorState& state)
	{
		m_lightSensorState = state;
	}

	/// <summary>
	/// ������ݒ�B
	/// </summary>
	void SetDirection(const Vector3& dir)
	{
		m_direction = dir;
	}

	/// <summary>
	/// �ړ����x��ݒ�B
	/// </summary>
	void SetMoveSpeed(const Vector3& movSpd)
	{
		m_moveSpeed = movSpd;
	}

	/// <summary>
	/// ���ˊp�x��ݒ�B
	/// </summary>
	void SetAngle(const float angle)
	{
		m_angle = angle;
	}

	/// <summary>
	/// �ő厞�Ԃ�ݒ�B
	/// </summary>
	void SetMaxTime(const float time)
	{
		m_maxTime = time;
	}

private:
	/// <summary>
	/// �^�C���̌v�������B
	/// </summary>
	void Time();

	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();

	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotate();

	void Hit() override
	{

	}

private:
	SpotLight			m_spotLight;			//�X�|�b�g���C�g
	Gage*				m_gage = nullptr;		//�Q�[��UI
	LightSensorState	m_lightSensorState = enState_Move;
	Vector3				m_direction;			//����
	Vector3				m_moveSpeed;			//�ړ����x
	bool				m_isTimeOver = false;	//�^�C���𒴂�����
	float				m_angle = 25.0f;		//���ˊp�x
	float				m_timer = 0.0f;			//����
	float				m_maxTime = 5.0f;		//�ő厞��
};

