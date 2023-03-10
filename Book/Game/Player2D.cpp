#include "stdafx.h"
#include "Player2D.h"
namespace
{
	const Vector3 BOXSIZE{ 80.0f,120.0f,2.0f };//ボックスコライダーの大きさ
	const Vector3 MODELSIZE{ 100.0f,100.0f,100.0f };
}
Player2D::Player2D()
{

}
Player2D::~Player2D()
{

}
bool Player2D::Start()
{
	Player::Start();
	delete(m_characon);
	m_modelRender = new ModelRender;
	m_characon = new CharacterController;
	
	//モデルの読み込み
	m_modelRender->Init("Assets/modelData/player/player2D.tkm");
	m_modelRender->SetScale(MODELSIZE);
	m_characon->Init(BOXSIZE, m_position);
	m_player2D[0].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_1.DDS");
	m_player2D[1].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_2.DDS");
	m_player2D[2].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_3.DDS");
	return true;
}
void Player2D::Update()
{
	Player::Update();
	int j = i / 10;
	i++;
	if (i >= 30)
	{
		i = 0;
	}
	m_modelRender->GetModel().ChangeAlbedoMap("", m_player2D[j]);
	m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender->SetPosition(m_position);
	m_modelRender->Update();		
}
void Player2D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}