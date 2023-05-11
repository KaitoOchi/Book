#include "stdafx.h"
#include "Player2D.h"
#include "Player3D.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
#include "Star.h"
#include "Enemy.h"
#include "Game.h"
namespace
{
	const Vector3 BOXSIZE{ 20.0f,120.0f,2.0f };//ボックスコライダーの大きさ
	const Vector3 MODELSIZE{ 1.0f,1.0f,1.0f };
	int WALKVALUE = 30;
	int JUMPVALUE = 90;
	float RUBVALUM = 3.0f;
	const float SPEEDDOWN = 0.8;//速度減速率
}
Player2D::Player2D()
{

}
Player2D::~Player2D()
{
	if (IsActive())
	{

		delete(m_characon);
		delete(m_modelRender);
	}
}
bool Player2D::Start()
{

	Player::Start();
	//2Dアニメーションのロード
	Player::Animation2D();
	m_characon = new CharacterController;
	m_modelRender = new ModelRender;
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_player3D = FindGO<Player3D>("player3d");
	//モデルの読み込み
	m_modelRender->Init("Assets/modelData/player/player2D.tkm",0,0,enModelUpAxisZ, false, false, 0, D3D12_CULL_MODE_NONE);
	m_modelRender->SetScale(MODELSIZE);

	
	m_characon->Init(BOXSIZE, m_position);
	delete(m_characon);
	m_characon = nullptr;
	Deactivate();
	return true;
}
void Player2D::Update()
{
	
	//atan2を使用して回転角度を求める
	angle = atan2(-m_moveSpeed.z, -m_moveSpeed.x);

	Player::Update();
	Rotation2D();
	if (GetCharacon() == nullptr)
	{
		return;
	}

	
	m_characon->SetPosition(m_position);
	m_characon->SetRotaition(m_rotation);
	m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime()/2.0f);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->Update();	
}
void Player2D::Rotation2D()
{
	if (m_Lstic.y > 0.0f&&
		m_Lstic.x<0.5f&&
		m_Lstic.x>-0.5f)
	{
		m_rot.AddRotationY(90.0f);
	}
	if (m_Lstic.y < 0.0f &&
		m_Lstic.x<0.5f &&
		m_Lstic.x>-0.5f)
	{
		m_rot.AddRotationY(-90.0f);
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
//キャラコンの作成関数
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
		JUMPVALUE = 90;
		j = i / 10;
		i++;
		if (i >= 29)
		{
			i = 0;
			
		}
		break;
	case Player::m_enPlayer_walk:
		JUMPVALUE = 90;
		j = WALKVALUE / 10;
		WALKVALUE++;
		if (WALKVALUE >= 89)
		{
			WALKVALUE = 30;
			
		}
		break;
	case Player::m_enPlayer_Run:
		//歩くアニメーションを早くする
		j = RUBVALUM;

		RUBVALUM += 0.2;
		if (RUBVALUM >= 8.8)
		{
			RUBVALUM = 3;
			
		}
		break;
	case Player::m_enPlayer_Jump:
	case Player::m_enPlayer_Jumpend:
		j = JUMPVALUE / 10;
		JUMPVALUE++;
		if (JUMPVALUE >= 130)
		{
			JUMPVALUE = 130;
		}
		break;
	
		break;
	case Player::m_enPlayer_Change:
		break;
	case Player::m_enPlayer_2DChanging:
		break;
	case Player::m_enPlayer_3DChanging:
		break;
	case Player::m_enPlayer_Found:
		break;
	case Player::m_enPlayer_Caught:
		break;
	case Player::m_enPlayer_Clear:
		break;
	case Player::m_enPlayer_GameOver:
		break;
	default:
		break;
	}
	m_modelRender->GetModel().ChangeAlbedoMap("", m_player2D[j]);
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
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		m_playerState = m_enPlayer_Jumpend;

	}
}
void Player2D::ProcessJumpendStateTransition()
{

	if (m_modelRender->IsPlayingAniamtion() == false && m_characon->IsOnGround())
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}

}
void Player2D::ProcessChangeStateTransition()
{

	//ステートを遷移する。
	ProcessCommonStateTransition();
}
void Player2D::ProcessDownStateTransition()
{
}
void Player2D::ProcessThrowStateTransition()
{
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
	m_playerManagement->SetChange(m_playerManagement->m_enPlayer_3DChanging);
	m_player3D->SetPlayerState(m_player3D->m_enPlayer_Caught);
	m_playerState = m_enPlayer_Catching;
	
}

void Player2D::ProcessCatchingStateTransition()
{

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