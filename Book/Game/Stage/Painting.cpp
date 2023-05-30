#include "stdafx.h"
#include "Painting.h"

Painting::Painting()
{

}

Painting::~Painting()
{

}

bool Painting::Start()
{
	Quaternion rot;
	rot.SetRotationDegX(180.0f);

	m_modelRender.Init("Assets/modelData/level_test/tkm/painting.tkm");
	m_modelRender.SetRotation(rot);
	m_modelRender.Update();

	m_modelRender.GetModel().ChangeAlbedoMap("",
		m_texture
		);

	m_frameModelRender.Init("Assets/modelData/level_test/tkm/frame.tkm");
	m_frameModelRender.Update();

	return true;
}

void Painting::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_frameModelRender.Draw(rc);
}