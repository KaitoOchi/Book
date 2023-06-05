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
	m_modelRender.Init("Assets/modelData/level_test/tkm/base.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	m_physicsStaticObj.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	return true;
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}