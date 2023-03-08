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
	//キャラコンを初期化する。
	m_characon.Init(25.0f, 75.0f, m_position);
	return true;
}

void Player::Update()
{
	
	Move();
	Jump();
	Rotation();
	ManageState();
	
}

void Player::Move()
{
	//左ステックの入力量を初期化
	m_Lstic.x = 0.0f;
	m_Lstic.z = 0.0f;
	//移動速度も初期化
	m_moveSpeed.x*=0.8f;
	m_moveSpeed.z *= 0.8f;
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
	//地面に付いていたら。
	if (m_characon.IsOnGround()==true)
	{
		//重力を無くす。
		m_moveSpeed.y = 0.0f;
		//Bボタンが押されたら。
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//ジャンプさせる。
			m_moveSpeed.y = m_jumpvolume;
			
		}
	}
	//地面に付いていなかったら。
	else
	{
		//重力を発生させる。
		m_moveSpeed.y -= 400.0f * g_gameTime->GetFrameDeltaTime();
	}
}

void Player::Rotation()
{
	//動いていないため回転しない
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}
	//atan2で角度を求めて、これを回転角度にしている
	angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//ラジアン単位のためSetRotaitionを使用する
	m_rotation.SetRotationY(-angle);
}
void Player::ProcessCommonStateTransition()
{
	if (m_characon.IsOnGround() == false)
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