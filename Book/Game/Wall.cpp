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
	m_wallRender.Init("Assets/modelData/level_test/box.tkm");
	m_wallRender.SetPosition(m_position);
	m_wallRender.SetScale(m_scale);
	m_wallRender.SetRotation(m_rotation);
	m_wallRender.Update();
	m_colliob.CreateBox(
		m_position,			//座標
		m_rotation,         //回転
	    m_scale				//大きさ
	);
	//m_physicsStaticObj.CreateFromModel(m_wallRender.GetModel(), m_wallRender.GetModel().GetWorldMatrix());
	//m_physicsStaticObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);	// コリジョン属性を設定
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