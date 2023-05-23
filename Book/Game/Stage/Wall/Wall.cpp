#include "stdafx.h"
#include "Wall.h"

#include "PlayerManagement.h"
#include "GameManager.h"

namespace
{
	const int GAP_SUM = 1;		// ���Ԃ���̎��
	const int PAINT_SUM = 1;	// �G�悠��̎��
}

Wall::Wall()
{

}
Wall::~Wall()
{

}
bool Wall::Start()
{
	m_player = FindGO<PlayerManagement>("playerManagement");

	m_wallRender.SetPosition(m_position);
	m_wallRender.SetRotation(m_rotation);
	m_wallRender.SetScale(m_scale);
	m_wallRender.Update();

	return true;
}

void Wall::CreateBoxObj()
{
	// 当たり判定を作成する
	m_physicsBoxObj.CreateFromModel(m_wallRender.GetModel(), m_wallRender.GetModel().GetWorldMatrix(), m_scale);
	m_physicsBoxObj.GetRigidBody()->SetPositionAndRotation(Vector3(m_position.x,m_position.y+100.0f,m_position.z), m_rotation);
	m_physicsBoxObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);	// �R���W����������ݒ�
}

void Wall::WipeRender(RenderContext& rc)
{
	m_wallRender.Draw(rc);
}

void Wall::Render(RenderContext& rc)
{
	if (GameManager::GetInstance()->GetGameState() == GameManager::enState_Result) {
		m_wallRender.Draw(rc);
		return;
	}

	Vector3 diff = m_RenderPosition - m_position;
	if (diff.Length() < 1500.0f) {
		m_wallRender.Draw(rc);
	}
}