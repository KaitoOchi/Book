#include "stdafx.h"
#include "GameCamera.h"

#include "Player3D.h"
#include "Player2D.h"
#include"PlayerManagement.h"
#include "Game.h"

namespace
{
	const Vector3 BEKUTORU(0.0f, 400.0f, -300.0f);	//カメラのベクトル
	const float TARGET_UP = 10.0f;					//注視点を上げる量

}

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//カメラの現在の座標
	m_toCameraPos = { 0.0f ,BEKUTORU.y, BEKUTORU.z };

	//インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_player3D = FindGO<Player3D>("player3d");
	m_game = FindGO<Game>("game");

	g_camera3D->SetNear(150.0f);
	g_camera3D->SetFar(12000.0f);

	return true;
}

void GameCamera::Update()
{
	if (m_player3D->m_playerState != m_player3D->m_enPlayer_Catching)
	{
		//カメラ座標の更新
		UpdatePositionAndTarget();
		
	}
	else
	{
		CatchMove();
	}

	//更新
	g_camera3D->Update();
}

void GameCamera::UpdatePositionAndTarget()
{
	if (m_playerManagement->IsDead() ||
		m_playerManagement->m_enMananagementState == PlayerManagement::m_enPlayer_Changing)
	{
		SetTarget({m_playerChangeCameraPosition.x,m_playerChangeCameraPosition.y + 100.0f,m_playerChangeCameraPosition.z });
		return;
	}

	SetTarget({ m_playerManagement->GetPosition() .x,m_playerManagement->GetPosition().y+100.0f,m_playerManagement->GetPosition().z});
	//注視点のY座標を上げる
	m_target += Vector3(0.0f, TARGET_UP, 0.0f);
	//ステックの入力量を求める
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	qRot.SetRotationDeg(Vector3::AxisY, 1.5f * x);
	qRot.Apply(m_toCameraPos);
	//視点をプレイヤーが向いている方向にする
	if (g_pad[0]->IsTrigger(enButtonRB3)&&
		m_game->m_gameState!=Game::m_enGameState_GameStart()&&
		m_pushState)
	{
		RotCamera();
	}
	m_camePos = m_target + m_toCameraPos;
	//カメラの注視点と座標を更新
	g_camera3D->SetPosition(m_camePos);
	g_camera3D->SetTarget(m_target);

}

void GameCamera::CatchMove()
{
	//カメラの動かす量を求める
	m_cameraMove += g_gameTime->GetFrameDeltaTime() * 0.5f;
	//プレイヤーの座標取得する
	Vector3 m_playerPosition = m_player3D->GetPosition();
	m_playerPosition.y += 100.0f;
	//Lerp関数によりカメラの動きを線形補完する
	m_caughtCameraPosition.Lerp(m_cameraMove, m_target+m_toCameraPos, m_playerPosition);
	if (m_cameraMove > 0.6f)
	{
		//ゲームオーバーを通知する
		m_game->NotifyGameOver();
		return;
	}
	g_camera3D->SetPosition(m_caughtCameraPosition);
}

void GameCamera::RotCamera()
{
	//プレイヤーの移動速度を取得する
	Vector3 pos = m_playerManagement->GetMoveSpeed();
	pos.y = 0.0f;
	//正規化して反転する
	pos.Normalize();
	pos *= -1.0f;
	//カメラの座標をプレイヤーの移動速度を代入する
	m_toCameraPos = { pos.x * 300.0f ,BEKUTORU.y,pos.z * 300.0f };
}