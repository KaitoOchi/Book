#include "stdafx.h"
#include "Wall.h"
Wall::Wall()
{

}
Wall::~Wall()
{

}
bool Wall::Start()
{
	//m_wallRender.Init("Assets/modelData/level_test/box.tkm");
	m_wallRender.Init("Assets/modelData/level_test/wall1.tkm");
	m_wallRender.SetPosition(m_position);
	m_wallRender.SetScale(m_scale);
	m_wallRender.SetRotation(m_rotation);

	m_physicsStaticObj.CreateFromModel(m_wallRender.GetModel(), m_wallRender.GetModel().GetWorldMatrix());
	m_physicsStaticObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);	// ƒRƒŠƒWƒ‡ƒ“‘®«‚ğİ’è
	
	/*m_physicsBoxObj.CreateFromModel(m_wallRender.GetModel(), m_wallRender.GetModel().GetWorldMatrix(),m_scale);
	m_physicsBoxObj.Create(m_position, m_rotation);*/
	m_wallRender.Update();
	return true;
}
void Wall::Update()
{
	m_wallRender.Update();
}
void Wall::Render(RenderContext& rc)
{
	m_wallRender.Draw(rc);
}