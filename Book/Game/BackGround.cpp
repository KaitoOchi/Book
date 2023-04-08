#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
}

BackGround::~BackGround()
{
}

bool BackGround::Start()
{
	// モデルの読み込み
	m_ModelRender.Init("Assets/modelData/level_test/base.tkm");
	m_ModelRender.SetScale(m_scale);
	m_ModelRender.SetPosition(m_position);
	m_ModelRender.SetRotation(m_rotation);

	m_physicsStaticObj.CreateFromModel(m_ModelRender.GetModel(), m_ModelRender.GetModel().GetWorldMatrix());
	m_physicsStaticObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);	// コリジョン属性を設定

	return true;
}

void BackGround::Render(RenderContext& rc)
{
	m_ModelRender.Draw(rc);
}