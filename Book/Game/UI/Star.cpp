#include "stdafx.h"
#include "Star.h"
#include "Player3D.h"
#include "PlayerManagement.h"
namespace
{
	const Vector3 STARSCALE{ 0.1f,0.1f,0.1f };
}
Star::Star()
{

}

Star::~Star()
{

}

bool Star::Start()
{
	m_starRender.Init("Assets/modelData/Status/star/star.tkm");
	m_starRender.SetPosition(m_position);
	m_starRender.SetScale(STARSCALE);
	m_starRender.Update();
	Deactivate();
	return true;
}

void Star::Update()
{
	Rotaition();
	m_starRender.SetPosition(GetPosition());
	m_starRender.Update();
}

void Star::Rotaition()
{
	//ÅôÇâÒÇµë±ÇØÇÈ
	m_rot.SetRotationDegY(2.0f);
	m_starRender.SetRotation(m_rot);
}

void Star::Render(RenderContext& rc)
{
	m_starRender.Draw(rc);
}