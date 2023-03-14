#include "stdafx.h"
#include "SenSor.h"


SenSor::SenSor()
{

}

SenSor::~SenSor()
{

}

bool SenSor::Start()
{
	m_player = FindGO<Player3D>("player3d");

	m_position = Vector3(200.0f, 50.0f, 0.0f);
	m_scale = Vector3(1.0f, 1.0f, 50.0f);

	m_modelRender.Init("Assets/modelData/object/sensor.tkm");
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
	//ÉvÉåÉCÉÑÅ[Ç∆ê⁄êG
	int a = 0;
	a = 1;
}


void SenSor::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}