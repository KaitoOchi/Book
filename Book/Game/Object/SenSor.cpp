#include "stdafx.h"
#include "Sensor.h"

#include "Gage.h"

namespace
{
	const int GAGE = 2;		//�㏸����x���l
}


SenSor::SenSor()
{

}

SenSor::~SenSor()
{
	DeleteGO(m_collisionObject);
}

bool SenSor::Start()
{
	m_player = FindGO<PlayerManagement>("playerManagement");
	m_gage = FindGO<Gage>("gage");

	m_modelRender.Init("Assets/modelData/object/sensor/sensor.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(m_position, m_rotation, m_scale);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("sensor");

	return true;
}

void SenSor::Update()
{
	Collision();
}

void SenSor::Hit()
{
	//�v���C���[�ƐڐG
	m_gage->GageUp(GAGE);
}

void SenSor::Render(RenderContext& rc)
{
	if (m_isActive)
		m_modelRender.Draw(rc);
}