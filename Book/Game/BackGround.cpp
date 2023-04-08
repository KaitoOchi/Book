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
	// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_ModelRender.Init("Assets/modelData/level/debug.tkm", 0, 0, enModelUpAxisZ, true, D3D12_CULL_MODE_BACK, false);
	m_ModelRender.SetScale(m_scale);
	m_ModelRender.SetPosition(m_position);
	m_ModelRender.SetRotation(m_rotation);

	m_physicsStaticObj.CreateFromModel(m_ModelRender.GetModel(), m_ModelRender.GetModel().GetWorldMatrix());
	m_physicsStaticObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);	// ƒRƒŠƒWƒ‡ƒ“‘®«‚ğİ’è

	return true;
}

void BackGround::Render(RenderContext& rc)
{
	m_ModelRender.Draw(rc);
}