#include "stdafx.h"
#include "Treasure.h"
Treasure::Treasure()
{

}
Treasure::~Treasure()
{

}
bool Treasure::Start()
{
	//ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_modelRender.Init("Assets/modelData/object/takara/item.tkm");
	m_modelRender.SetPosition(Vector3::Zero);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
	return true;
}
void Treasure::Update()
{

	m_modelRender.Update();
}
void Treasure::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}