#include "stdafx.h"
#include "Player2D.h"
#include "Player3D.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
#include "Enemy.h"
#include "Game.h"
#include "GameUI.h"
namespace
{
	const Vector3	BOXSIZE{ 20.0f,120.0f,2.0f };						//ボックスコライダーの大きさ
	const Vector3	MODELSIZE{ 1.0f,1.0f,1.0f };						//モデルサイズ
	const float		SPEED_DOWN = 0.8;									//速度減速率
	const float		PLAYER_STAMINA = 10.0f;								//プレイヤーのスタミナ
	const float		EFFECT_SIZE = 1.5f;									//エフェクトサイズ
}
Player2D::Player2D()
{

}
Player2D::~Player2D()
{
	if (IsActive())
	{
		delete m_changeAnimation;
		delete(m_characon);
		delete(m_modelRender);
	}
}
bool Player2D::Start()
{
	m_changeAnimation[enAnimationClip_Idle].Load("Assets/animData/player_2D/player2D_idle.tka");
	m_changeAnimation[enAnimationClip_Idle].SetLoopFlag(false);
	m_changeAnimation[enAnimationClip_Change].Load("Assets/animData/player_2D/player2D_change.tka");
	m_changeAnimation[enAnimationClip_Change].SetLoopFlag(false);

	m_characon = new CharacterController;
	m_modelRender = new ModelRender;
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_player3D = FindGO<Player3D>("player3d");
	//モデルの読み込み
	m_modelRender->Init("Assets/modelData/player/player2D.tkm", m_changeAnimation, enAnimationClip_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_2DPlayer, D3D12_CULL_MODE_NONE);
	m_modelRender->SetScale(MODELSIZE);
	m_modelRender->PlayAnimation(enAnimationClip_Idle, 0.0f);
	m_modelRender->Update();

	
	m_characon->Init(BOXSIZE, m_position);
	delete(m_characon);
	m_characon = nullptr;

	Player::Start();

	Deactivate();

	return true;
}
void Player2D::Update()
{
	Player::Update();
	if (GetCharacon() == nullptr)
	{
		return;
	}

	//キャラコンを動かす
	m_characon->SetPosition(m_position);
	//キャラコンを回転させる
	m_characon->SetRotaition(m_rotation);
	//キャラコンを利用してプレイヤーを動かす
	m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime()/2.0f);

	//atan2を使用して回転角度を求める
	Player::Rotation2D();

	//モデルの座標と回転を更新する
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->Update();	

	if (!GetRunState())
	{
		m_gameUI->SetChangeGaugeState(false);
	}
	else
	{
		m_gameUI->SetChangeGaugeState(true);
	}
}
void Player2D::PlayerChang()
{
	delete(m_characon);
	m_characon = nullptr;
	//ステートを遷移する。
	ProcessCommonStateTransition();
	Deactivate();
}

void Player2D::CreatCharcon()
{
	m_characon = new CharacterController;
	m_characon->Init(BOXSIZE, GetPosition());
}

void Player2D::Animation()
{
	switch (m_playerState)
	{
	case Player::m_enPlayer_Idle:
		m_addValue++;
		m_playAnimationSpeed = m_addValue / 10;
		if (m_addValue >= 29)
		{
			m_addValue = 0;
		}
		break;
	case Player::m_enPlayer_walk:
		//歩くアニメーションの切り替え
		m_walkAnimationSpeed++;
		m_playAnimationSpeed = m_walkAnimationSpeed / 10;
		if (m_walkAnimationSpeed >= 89)
		{
			m_walkAnimationSpeed = 30;
		}
		break;
	case Player::m_enPlayer_Run:
		//歩くアニメーションを早くする
		m_playAnimationSpeed = m_runAnimationSpeed;

		m_runAnimationSpeed += 0.2;
		if (m_runAnimationSpeed >= 8.8)
		{
			m_runAnimationSpeed = 3.0f;
		}
		break;
	case Player::m_enPlayer_Jump:
	case Player::m_enPlayer_Jumpend:
		//ジャンプアニメーション切り替え
		m_playAnimationSpeed = m_jumpAnimationSpeed / 10;
		m_jumpAnimationSpeed++;
		if (m_jumpAnimationSpeed >= 130)
		{
			m_jumpAnimationSpeed = 130;
		}
		break;
	case Player::m_enPlayer_Change:
		m_modelRender->PlayAnimation(enAnimationClip_Change, 0.0f);
		break;
	default:
		break;
	}

	RenderingEngine::GetInstance()->GetLightCB().shadowCB.playerAnim2D = m_playAnimationSpeed;
}

void Player2D::ProcessIdleStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player2D::ProcessWalkStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player2D::ProcessRunStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player2D::ProcessJumpStateTransition()
{
	m_playerState = m_enPlayer_Jumpend;
}
void Player2D::ProcessJumpendStateTransition()
{
	if (m_characon->IsOnGround())
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}

}
void Player2D::ProcessChangeStateTransition()
{
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
		m_modelRender->PlayAnimation(enAnimationClip_Idle, 0.0f);
		m_modelRender->Update();
	}
}
void Player2D::ProcessFoundStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player2D::ProcessStealStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player2D::ProcessCaughtStateTransition()
{
	if (!m_chactState2D)
	{
		m_playerManagement->SetChange(m_playerManagement->m_enPlayer_3DChanging);
		m_player3D->SetPlayerState(m_player3D->m_enPlayer_Caught);
		m_playerState = m_enPlayer_Catching;
		m_chactState2D = true;
	}
}

void Player2D::ProcessClearStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player2D::ProcessGameOverStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player2D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}

void Player2D::Throw()
{
}
void Player2D::ProcessDownStateTransition()
{
}
void Player2D::ProcessThrowStateTransition()
{
}
void Player2D::ProcessCatchingStateTransition()
{
}