#include "stdafx.h"
#include "Player3D.h"
Player3D::Player3D()
{

}
Player3D::~Player3D()
{

}
bool Player3D::Start()
{
	m_model3DRender.Init("Assets/modelData/unityChan.tkm");
	//�L�����R��������������B
	m_3Dcharacon.Init(25.0f, 75.0f, m_position);
	m_model3DRender.SetPosition(m_position);
	m_model3DRender.SetRotation(Quaternion::Identity);
	m_model3DRender.SetScale(Vector3::One);
	m_model3DRender.Update();
	return true;
}

void Player3D::Update() 
{
	Player::Update();
	//�v���C���[�̈ړ����p������B
	//�L�����R���ō��W���ړ�������B
	m_position = m_3Dcharacon.Execute(m_moveSpeed, 1.0f / 60.0f);
	m_model3DRender.SetPosition(m_position);
	m_model3DRender.Update();

}

void Player3D::Render(RenderContext& rc)
{
	m_model3DRender.Draw(rc);
}