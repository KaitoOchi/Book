#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
namespace
{
	const Vector3 BEKUTORU(0.0f, 100.0f, -200.0f);//�����_���王�_�܂ł̃x�N�g����ݒ�B
	const float TAGETUP = 80.0f;//�����_���グ���
	const float FRONTO = 20.0f;//�J�����̑O�����̗ʂ�ύX�ł���
	const float YUP = -0.5f;//�J�����̏�����̌��E
	const float YDOWN = 0.8f;//�J�����̉������̌��E
	const float XRIGHT = 0.8f;//�J�����̉E�����̌��E
	const float XLEFT = -0.5f;//�J�����̍������̌��E
}
GameCamera::GameCamera()
{

}
GameCamera::~GameCamera()
{

}
bool GameCamera::Start()
{
	//�����_���王�_�܂ł̃x�N�g����ݒ�
	m_toCameraPos.Set(BEKUTORU);
	//�v���C���[�̃C���X�^���X
	m_player = FindGO<Player>("player");
	return true;
}
void GameCamera::Update()
{
	//���_�ƒ����_�̍X�V
	UpdatePositionAndTarget();
	//�J�����̍X�V
	g_camera3D->Update();
}
void GameCamera::UpdatePositionAndTarget()
{
	target = m_player->GetPosition();
	//�v���C���[�̑������炿����Ə�𒍎��_�Ƃ���
	target.y += TAGETUP;
	target += g_camera3D->GetForward() * FRONTO;

	m_toCameraPosOld = m_toCameraPos;
	//�E�X�e�b�N�ŉ�
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.5f * y);
	qRot.Apply(m_toCameraPos);
	//�J�����̉�]�̏�����`�F�b�N����B
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < YUP)
	{
		//���������
		m_toCameraPos = m_toCameraPosOld;
	}
	else if (toPosDir.y > YDOWN)
	{
		//���������
		m_toCameraPos = m_toCameraPosOld;
	}
	if (toPosDir.x < XRIGHT)
	{
		//���������
		m_toCameraPos = m_toCameraPosOld;
	}
	else if (toPosDir.x > XLEFT)
	{
		//���������
		m_toCameraPos = m_toCameraPosOld;
	}
	//���_�̌v�Z
	Vector3 pos = target + m_toCameraPos;
	//�J�����ɒ����_�Ǝ��_��ݒ肷��
	g_camera3D->SetPosition(pos);
	g_camera3D->SetTarget(target);

}