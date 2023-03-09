#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
namespace
{
	const Vector3 BEKUTORU(0.0f, 100.0f, -200.0f);//注視点から視点までのベクトルを設定。
	const float TAGETUP = 80.0f;//注視点を上げれる
	const float FRONTO = 20.0f;//カメラの前方向の量を変更できる
	const float YUP = -0.5f;//カメラの上方向の限界
	const float YDOWN = 0.8f;//カメラの下方向の限界
	const float XRIGHT = 0.8f;//カメラの右方向の限界
	const float XLEFT = -0.5f;//カメラの左方向の限界
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
	//プレイヤーのインスタンス
	m_player = FindGO<Player>("player");
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
	target = m_player->GetPosition();
	//プレイヤーの足元からちょっと上を注視点とする
	target.y += TAGETUP;
	target += g_camera3D->GetForward() * FRONTO;

	m_toCameraPosOld = m_toCameraPos;
	//右ステックで回す
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.5f * y);
	qRot.Apply(m_toCameraPos);
	//カメラの回転の上限をチェックする。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < YUP)
	{
		//上向きすぎ
		m_toCameraPos = m_toCameraPosOld;
	}
	else if (toPosDir.y > YDOWN)
	{
		//上向きすぎ
		m_toCameraPos = m_toCameraPosOld;
	}
	if (toPosDir.x < XRIGHT)
	{
		//上向きすぎ
		m_toCameraPos = m_toCameraPosOld;
	}
	else if (toPosDir.x > XLEFT)
	{
		//上向きすぎ
		m_toCameraPos = m_toCameraPosOld;
	}
	//視点の計算
	Vector3 pos = target + m_toCameraPos;
	//カメラに注視点と視点を設定する
	g_camera3D->SetPosition(pos);
	g_camera3D->SetTarget(target);

}