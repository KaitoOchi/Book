#include "stdafx.h"
#include "Player.h"
namespace
{
	const float WALK = 20.0f;//歩き時の乗算量
	const float RUN = 40.0f;//走り時の乗算量
	const float JUMPVOLUM = 150.0f;//ジャンプ量
}

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	m_characon.Init(25.0f, 75.0f, m_position);
	return true;
}

void Player::Update()
{
	if (m_playerState!=m_enPlayer3D_Throw) {
		Move();
		Jump();
		Rotation();
	}
	
	
	ManageState();

}

void Player::Move()
{
	m_Lstic.x = 0.0f;
	m_Lstic.z = 0.0f;
	//速度を初期化
	m_moveSpeed.x *= 0.8f;
	m_moveSpeed.z *= 0.8f;
	//左ステックの情報を取得
	m_Lstic.x = g_pad[0]->GetLStickXF();
	m_Lstic.z = g_pad[0]->GetLStickYF();
	//もしAボタンが押されているなら
	if (g_pad[0]->IsPress(enButtonA))
	{
		//ダッシュをさせる
		//左ステックと走る速度を乗算する
		m_moveSpeed.x += m_Lstic.x * RUN;
		m_moveSpeed.z += m_Lstic.z * RUN;
	}
	else
	{
		//左ステックと歩く速度を乗算させる
		m_moveSpeed.x += m_Lstic.x * WALK;
		m_moveSpeed.z += m_Lstic.z * WALK;
	}

}
void Player::Jump()
{
	//もし地面についているなら
	if (m_characon.IsOnGround() == true)
	{
		//重力を無くす
		m_moveSpeed.y = 0.0f;
		//もしBボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//ジャンプをする
			m_moveSpeed.y = JUMPVOLUM;

		}
	}
	//空中にいる場合
	else
	{
		//重力を発生させる
		m_moveSpeed.y -= 400.0f * g_gameTime->GetFrameDeltaTime();
	}
}
void Player::Rotation()
{
	//もし少しも動いていないなら
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}
	//atan2を使用して回転角度を求める
	angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//SetRotationを使用する
	m_rotation.SetRotationY(-angle);
}
void Player::ProcessCommonStateTransition()
{
	if (m_characon.IsOnGround() == false)
	{
		//ジャンプ中にする
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
			//歩き中にする
			m_playerState = m_enPlayer_walk;
		}
	}
	else
	{
		//待機中にする
		m_playerState = m_enPlayer_Idle;
	}

}
void Player::ProcessIdleStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player::ProcessWalkStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player::ProcessRunStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player::ProcessJumpStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player::ProcessJumpendStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player::ProcessChangeStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player::ProcessThrowStateTransition()
{
	//速度を初期化
	m_moveSpeed.x *= 0.8f;
	m_moveSpeed.z *= 0.8f;
	if (m_modelRender.IsPlayingAniamtion() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}
void Player::ManageState()
{
	switch (m_playerState)
	{
		//待機ステートの時
	case m_enPlayer_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//歩きステートの時
	case m_enPlayer_walk:
		//歩きステートのステート遷移処理。
		ProcessWalkStateTransition();
		break;
		//ダッシュステートの時
	case m_enPlayer_Run:
		//ダッシュステートのステート遷移処理。
		ProcessRunStateTransition();
		break;
		//ジャンプステートの時
	case m_enPlayer_Jump:
		//ジャンプステートのステート遷移処理。
		ProcessJumpStateTransition();
		break;
		//ジャンプ終わりステートの時
	case m_enPlayer_Jumpend:
		//ジャンプ終わりのステート遷移処理。
		ProcessJumpendStateTransition();
		break;
		//切替ステートの時
	case m_enPlayer_Change:
		//切替ステートのステート遷移処理。
		ProcessChangeStateTransition();
		break;
		//投げるとき
	case m_enPlayer3D_Throw:
		ProcessThrowStateTransition();
		break;
	default:
		break;
	}
}