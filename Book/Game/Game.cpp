#include "stdafx.h"
#include "Game.h"
#include"Player3D.h"
#include"Player2D.h"
#include"GameCamera.h"
#include "PlayerManagement.h"
#include "Title.h"
#include "SenSor.h"


Game::Game()
{
	//“–‚½‚è”»’è‚ð—LŒø‰»
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

Game::~Game()
{
	DeleteGO(m_player3D);
	DeleteGO(m_player2D);
	DeleteGO(m_gamecamera);

}

bool Game::Start()
{
	m_player2D=NewGO<Player2D>(0,"player2d");
	m_player3D = NewGO<Player3D>(0, "player3d");
	m_gamecamera=NewGO<GameCamera>(0, "gameCamera");
	NewGO<SenSor>(0, "senSour");
	NewGO<PlayerManagement>(0,"playerManagement");
	m_stageModelRender.Init("Assets/modelData/stage1.tkm");
	m_stageModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_stageModelRender.SetRotation(Quaternion::Identity);
	m_stageModelRender.SetScale(Vector3::One);
	m_stageModelRender.Update();
	m_demobg.CreateFromModel(m_stageModelRender.GetModel(), m_stageModelRender.GetModel().GetWorldMatrix());
	return true;
}

void Game::Update()
{
	MnageState();
}
void Game::MnageState()
{
	if (g_pad[0]->IsPress(enButtonLB2))
	{
		m_gameState = m_enGameState_GameClear;
	}
	if (g_pad[0]->IsPress(enButtonRB2)&&m_gameState==m_enGameState_GameClear)
	{
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}
void Game::Render(RenderContext& rc)
{
	m_stageModelRender.Draw(rc);
}
