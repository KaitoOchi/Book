#include "stdafx.h"
#include "Game.h"
#include"Player3D.h"


Game::Game()
{
	//“–‚½‚è”»’è‚ð—LŒø‰»
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

Game::~Game()
{
	//b
}

bool Game::Start()
{
	NewGO<Player3D>(0,"player3d");
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
	
}
void Game::Render(RenderContext& rc)
{
	m_stageModelRender.Draw(rc);
}
