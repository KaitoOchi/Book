#include "stdafx.h"
#include "Player2D.h"
#include "Player3D.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
namespace
{
	const Vector3 BOXSIZE{ 80.0f,120.0f,2.0f };//ボックスコライダーの大きさ
	const Vector3 MODELSIZE{ 1.0f,1.0f,1.0f };
	int WALKVALUE = 30;
	int JUMPVALUE = 90;
}
Player2D::Player2D()
{

}
Player2D::~Player2D()
{
	delete(m_modelRender);
}
bool Player2D::Start()
{

	Player::Start();
	m_characon = new CharacterController;
	m_modelRender = new ModelRender;
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	//モデルの読み込み
	m_modelRender->Init("Assets/modelData/player/player2D.tkm");
	m_modelRender->SetScale(MODELSIZE);
	m_characon->Init(BOXSIZE, m_position);
	m_player2D[0].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_1.DDS");
	m_player2D[1].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_2.DDS");
	m_player2D[2].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_3.DDS");
	m_player2D[3].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_1.DDS");
	m_player2D[4].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_2.DDS");
	m_player2D[5].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_3.DDS");
	m_player2D[6].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_4.DDS");
	m_player2D[7].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_5.DDS");
	m_player2D[8].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_6.DDS");
	m_player2D[9].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_1.DDS");
	m_player2D[10].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_2.DDS");
	m_player2D[11].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_3.DDS");
	m_player2D[12].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_4.DDS");
	m_player2D[13].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_5.DDS");
	delete(m_characon);
	m_characon = nullptr;
	Deactivate();
	return true;
}
void Player2D::Update()
{
	if (GetCharacon() == nullptr)
	{
		return;
	}

	//atan2を使用して回転角度を求める
	angle = atan2(m_moveSpeed.z, -m_moveSpeed.x);

	Player::Update();
	Animation();
	Rotation2D();
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
		j = i / 10;
		i++;
		if (i >= 29)
		{
			i = 0;
		}
		break;
	case Player::m_enPlayer_walk:
		j = WALKVALUE / 10;
		WALKVALUE++;
		if (WALKVALUE >= 89)
		{
			WALKVALUE = 30;
		}
		break;
	case Player::m_enPlayer_Run:
		break;
	case Player::m_enPlayer_Jump:
		j = JUMPVALUE / 10;
		JUMPVALUE++;
		if (JUMPVALUE >= 130)
		{
			JUMPVALUE = 90;
		}
		break;
	case Player::m_enPlayer_Jumpend:
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
	
void Player2D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}