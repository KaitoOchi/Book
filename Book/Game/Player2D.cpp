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
	m_player2D[0].InitFromDDSFile(L"Assets/animData/Player_2D/idle/idle_1.dds");
	m_player2D[1].InitFromDDSFile(L"Assets/animData/Player_2D/idle/idle_2.dds");
	m_player2D[2].InitFromDDSFile(L"Assets/animData/Player_2D/idle/idle_3.dds");
	return true;
}
void Player2D::Update()
{
	int j = i / 10;
	i++;
	if (i >= 30)
	{
		i = 0;
	}
	m_modelRender->GetModel().ChangeAlbedoMap("", m_player2D[j]);
	m_modelRender->SetPosition(m_position / 2.0f);
	m_modelRender->Update();		
}
void Player2D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}