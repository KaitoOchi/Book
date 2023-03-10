#include "stdafx.h"
#include "Player2D.h"
Player2D::Player2D()
{

}
Player2D::~Player2D()
{

}
bool Player2D::Start()
{
	m_modelRender = new ModelRender;
	Player::Start();
	//ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý
	m_modelRender->Init("Assets/modelData/player/player2D.tkm");
	m_modelRender->SetScale(Vector3(5.0f,5.0f,5.0f));
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
	m_position = m_characon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender->SetPosition(m_position);
	m_modelRender->Update();		
}
void Player2D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}