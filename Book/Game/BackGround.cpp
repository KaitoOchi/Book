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
	// ���f���̓ǂݍ���
	m_ModelRender.Init("Assets/modelData/level/debug.tkm");
	m_ModelRender.SetScale(m_scale);
	m_ModelRender.SetPosition(m_position);
	m_ModelRender.SetRotation(m_rotation);

	m_physicsStaticObj.CreateFromModel(m_ModelRender.GetModel(), m_ModelRender.GetModel().GetWorldMatrix());
	m_physicsStaticObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);	// �R���W����������ݒ�

	return true;
}

void BackGround::Render(RenderContext& rc)
{
	m_ModelRender.Draw(rc);
}