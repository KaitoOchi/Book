#include "stdafx.h"
#include "Player.h"


Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{

	return true;
}

void Player::Update()
{
	Move();
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		Jump();
	}
	ManageState();
}

void Player::Move()
{
	//左ステックの入力量を初期化
	m_Lstic.x = 0.0f;
	m_Lstic.z = 0.0f;
	//移動速度も初期化
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed.y = 0.0f;
	//左ステックの入力量を取得1
	m_Lstic.x = g_pad[0]->GetLStickXF();
	m_Lstic.z = g_pad[0]->GetLStickYF();
	//もしBボタンが押されているなら
	if (g_pad[0]->IsPress(enButtonA))
	{
		//ダッシュ中にする
		//移動速度に入力量を加算する
		m_moveSpeed.x += m_Lstic.x * m_run;
		m_moveSpeed.z += m_Lstic.z * m_run;
	}
	else
	{
		//移動速度に入力量を加算する
		m_moveSpeed.x += m_Lstic.x * m_walk;
		m_moveSpeed.z += m_Lstic.z * m_walk;
	}
	
}
void Player::Jump()
{
	m_moveSpeed.y = 0.0f;
	if (m_playerState != m_enPlayer_Jump)
	{
		m_moveSpeed.y += m_jumpvolume;
		m_playerState = m_enPlayer_Jumping;
	}
	m_position.y += m_moveSpeed.y;
}
void Player::ProcessCommonStateTransition()
{
	if (m_moveSpeed.y!=0.0f)
	{
		//ステートをジャンプ中にする
		m_playerState = m_enPlayer_Jump;
	}
	else if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		if (g_pad[0]->IsPress(enButtonA))
		{
			//ダッシュ中にする
			m_playerState = m_enPlayer_Run;
		}
		else
		{
			//歩き中にする。
			m_playerState = m_enPlayer_walk;
		}
	}
	else
	{
		//ステートを待機中にする
		m_playerState = m_enPlayer_Idle;
	}
	
}
void Player::ProcessIdleStateTransition()
{
	//ステートの遷移処理
	ProcessCommonStateTransition();
}
void Player::ProcessWalkStateTransition()
{
	//ステートの遷移処理
	ProcessCommonStateTransition();
}
void Player::ProcessRunStateTransition()
{
	//ステートの遷移処理
	ProcessCommonStateTransition();
}
void Player::ProcessJumpStateTransition()
{
	//ステートの遷移処理
	ProcessCommonStateTransition();
}
void Player::ProcessJumpingStateTransition()
{
	//ステートの遷移処理
	ProcessCommonStateTransition();
}
void Player::ProcessChangeStateTransition()
{
	//ステートの遷移処理
	ProcessCommonStateTransition();
}
void Player::ManageState()
{
	switch (m_playerState)
	{
	//待機ステートの時
	case m_enPlayer_Idle:
		//待機ステートのステート遷移処理
		ProcessIdleStateTransition();
		break;
	//歩きステートの時
	case m_enPlayer_walk:
		//歩きステートのステート遷移処理
		ProcessWalkStateTransition();
		break;
	//走りステートの時
	case m_enPlayer_Run:
		//走りステートのステート遷移処理
		ProcessRunStateTransition();
		break;
	//ジャンプステートの時
	case m_enPlayer_Jump:
		//ジャンプステートのステート遷移処理
		ProcessJumpStateTransition();
		break;
		//ジャンプ中ステートの時
	case m_enPlayer_Jumping:
		//ジャンプステートのステート遷移処理
		ProcessJumpingStateTransition();
		break;
	//切替ステートの時
	case m_enPlayer_Change:
		//切替ステートのステート遷移処理
		ProcessChangeStateTransition();
		break;
	default:
		break;
	}
}