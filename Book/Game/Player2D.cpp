#include "stdafx.h"
#include "Player2D.h"
#include "Player3D.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
namespace
{
	const Vector3 BOXSIZE{ 80.0f,120.0f,2.0f };//ボックスコライダーの大きさ
	const Vector3 MODELSIZE{ 1.0f,1.0f,1.0f };
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
	Player::Update();
	Animation();
	m_characon->SetPosition(m_position);
	m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime()/2.0f);
	m_modelRender->SetPosition(m_position);
	m_modelRender->Update();	
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
	int j = i / 10;
	i++;
	if (i >= 30)
	{
		i = 0;
	}
	m_modelRender->GetModel().ChangeAlbedoMap("", m_player2D[j]);
}
void Player2D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}