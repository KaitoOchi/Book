#include "stdafx.h"
#include "GameCamera.h"
#include "Player3D.h"
#include "Player2D.h"
#include"PlayerManagement.h"
namespace
{
	const Vector3 BEKUTORU(0.0f, 400.0f, -300.0f);//�����_���王�_�܂ł̃x�N�g����ݒ�B

	const float TAGETUP = 10.0f;//�����_��グ���

	const float FRONTO = 20.0f;//�J�����̑O�����̗ʂ�ύX�ł���
	const float YUP = -0.5f;//�J�����̏�����̌��E
	const float YDOWN = 0.8f;//�J�����̉������̌��E
	const float XRIGHT = 0.5f;//�J�����̉E�����̌��E
	const float XLEFT = -0.9f;//�J�����̍������̌��E
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
	//�v���C���[�Ǘ��̃C���X�^���X
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_player3D = FindGO<Player3D>("player3d");
	g_camera3D->SetNear(150.0f);
	g_camera3D->SetFar(3000.0f);

	return true;
}
void GameCamera::Update()
{
	if (m_player3D->m_playerState != m_player3D->m_enPlayer_Catching)
	{
		//���_�ƒ����_�̍X�V
		UpdatePositionAndTarget();
		//視点を元に戻す
		if (g_pad[0]->IsTrigger(enButtonLB2))
		{
			m_toCameraPos.Set(BEKUTORU);
		}
	}
	else
	{
		CatchMove();
	}
	//�J�����̍X�V
	g_camera3D->Update();
}
void GameCamera::UpdatePositionAndTarget()
{
	if (m_playerManagement->IsDead()||m_playerManagement->m_enMananagementState==PlayerManagement::m_enPlayer_Changing)
	{
		m_getPosition = m_cameraposition;

		SetPosition({m_cameraposition.x,m_cameraposition.y + 100.0f,m_cameraposition.z });
		return;
	}
	SetPosition({ m_playerManagement->GetPosition() .x,m_playerManagement->GetPosition().y+100.0f,m_playerManagement->GetPosition().z});
	//�v���C���[�̑������炿����Ə�𒍎��_�Ƃ���
	m_target += Vector3(0.0f, TAGETUP, 0.0f);
	m_toCameraPosOld = m_toCameraPos;
	//�E�X�e�b�N�ŉ�
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.5f * x);
	qRot.Apply(m_toCameraPos);
	//X������̉�]
	/*Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX,1.5f * y);
	qRot.Apply(m_toCameraPos);*/
	//�J�����̉�]�̏����`�F�b�N����B
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	//if (toPosDir.y < YUP)
	//{
	//	//���������
	//	m_toCameraPos = m_toCameraPosOld;
	//}
	//else if (toPosDir.y > YDOWN)
	//{
	//	//���������
	//	m_toCameraPos = m_toCameraPosOld;
	//}
	//if (toPosDir.x > XRIGHT)
	//{
	//	//���������
	//	m_toCameraPos = m_toCameraPosOld;
	//}
	//else if (toPosDir.x < XLEFT)
	//{
	//	//���������
	//	m_toCameraPos = m_toCameraPosOld;
	//}

	//���_�̌v�Z
	Vector3 pos = m_target + m_toCameraPos;
	//�J�����ɒ����_�Ǝ��_��ݒ肷��
	g_camera3D->SetPosition(pos);
	g_camera3D->SetTarget(m_target);

}

void GameCamera::CatchMove()
{
	m_cameraMove += g_gameTime->GetFrameDeltaTime() * 0.5f;
	Vector3 m_playerPosition = m_player3D->GetPosition();
	m_playerPosition.y += 100.0f;
	m_cameraPosition.Lerp(m_cameraMove, m_target+m_toCameraPos, m_playerPosition);
	if (m_cameraMove > 0.6f)
	{
		return;
	}
	g_camera3D->SetPosition(m_cameraPosition);
}