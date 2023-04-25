#include "stdafx.h"
#include "Star.h"
#include "Player3D.h"
#include "PlayerManagement.h"
namespace
{
	const Vector3 STARSCALE{ 5.0f,5.0f,5.0f };
}
Star::Star()
{

}

Star::~Star()
{

}

bool Star::Start()
{
	m_starRender.Init("Assets/modelData/object/takara/item.tkm");
	m_starRender.SetPosition(m_position);
	m_starRender.SetScale(STARSCALE);
	m_starRender.Update();
	Deactivate();
	return true;
}

void Star::Update()
{
	Rotaition();
	m_starRender.SetPosition(Vector3(m_position.x,m_position.y+=100.0f,m_position.z));
	m_starRender.Update();
}

void Star::Rotaition()
{
	//�����񂵑�����
	m_rot.AddRotationDegY(2.0f);
	m_starRender.SetRotation(m_rot);
}

void Star::Render(RenderContext& rc)
{
	m_starRender.Draw(rc);
}