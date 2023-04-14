#include "stdafx.h"
#include "GameCamera.h"
#include "Player3D.h"
#include "Player2D.h"
#include"PlayerManagement.h"
namespace
{
	const Vector3 BEKUTORU(0.0f, 400.0f, -300.0f);//注視点から視点までのベクトルを設定。

	const float TAGETUP = 10.0f;//注視点を上げれる

	const float FRONTO = 20.0f;//カメラの前方向の量を変更できる
	const float YUP = -0.5f;//カメラの上方向の限界
	const float YDOWN = 0.8f;//カメラの下方向の限界
	const float XRIGHT = 0.5f;//カメラの右方向の限界
	const float XLEFT = -0.9f;//カメラの左方向の限界
}
GameCamera::GameCamera()
{

}
GameCamera::~GameCamera()
{

}
bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定
	m_toCameraPos.Set(BEKUTORU);
	//プレイヤー管理のインスタンス
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	return true;
}
void GameCamera::Update()
{
	//視点と注視点の更新
	UpdatePositionAndTarget();
	//カメラの更新
	g_camera3D->Update();
}
void GameCamera::UpdatePositionAndTarget()
{
	SetPosition({ m_playerManagement->GetPosition() .x,m_playerManagement->GetPosition().y+100.0f,m_playerManagement->GetPosition().z});
	//プレイヤーの足元からちょっと上を注視点とする
	m_target += Vector3(0.0f, TAGETUP, 0.0f);
	m_target += g_camera3D->GetForward() * FRONTO;

	m_toCameraPosOld = m_toCameraPos;
	//右ステックで回す
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.5f * x);
	qRot.Apply(m_toCameraPos);
	//X軸周りの回転
	/*Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX,1.5f * y);
	qRot.Apply(m_toCameraPos);*/
	//カメラの回転の上限をチェックする。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	//if (toPosDir.y < YUP)
	//{
	//	//上向きすぎ
	//	m_toCameraPos = m_toCameraPosOld;
	//}
	//else if (toPosDir.y > YDOWN)
	//{
	//	//上向きすぎ
	//	m_toCameraPos = m_toCameraPosOld;
	//}
	//if (toPosDir.x > XRIGHT)
	//{
	//	//上向きすぎ
	//	m_toCameraPos = m_toCameraPosOld;
	//}
	//else if (toPosDir.x < XLEFT)
	//{
	//	//上向きすぎ
	//	m_toCameraPos = m_toCameraPosOld;
	//}
	if (g_pad[0]->IsTrigger(enButtonLB2))
	{
		m_toCameraPos.Set(BEKUTORU);
	}
	//視点の計算
	Vector3 pos = m_target + m_toCameraPos;
	//カメラに注視点と視点を設定する
	g_camera3D->SetPosition(pos);
	g_camera3D->SetTarget(m_target);

}