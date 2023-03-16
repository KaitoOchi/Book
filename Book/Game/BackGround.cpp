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
	// ÉÇÉfÉãÇÃì«Ç›çûÇ›
	m_ModelRender.Init("Assets/modelData/debugStage/debug.tkm");
	m_ModelRender.SetScale(m_scale);
	m_ModelRender.SetPosition(m_position);
	m_ModelRender.SetRotation(m_rotation);
	m_physicsStaticObj.CreateFromModel(m_ModelRender.GetModel(), m_ModelRender.GetModel().GetWorldMatrix());

	return true;
}

void BackGround::Render(RenderContext& rc)
{
	m_ModelRender.Draw(rc);
}