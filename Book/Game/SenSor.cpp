#include "stdafx.h"
#include "Sensor.h"


Sensor::Sensor()
{

}

Sensor::~Sensor()
{

}

bool Sensor::Start()
{
	m_player = FindGO<Player>("player");

	m_position = Vector3(-80.0f, 50.0f, 0.0f);
	m_scale = Vector3(1.0f, 1.0f, 50.0f);

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

void Sensor::Update()
{
	Collision();
}

void Sensor::Hit()
{
	//ÉvÉåÉCÉÑÅ[Ç∆ê⁄êG
	int a = 0;
	a = 1;
}

void Sensor::Render(RenderContext& rc)
{
	if (m_isActive)
		m_modelRender.Draw(rc);
}