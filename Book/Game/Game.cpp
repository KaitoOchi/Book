#include "stdafx.h"
#include "Game.h"
#include"Player3D.h"
#include"Player2D.h"
#include"GameCamera.h"
#include "PlayerManagement.h"
#include "Title.h"
#include "SenSor.h"
#include "MiniMap.h"
#include "Enemy_Normal.h"
#include "BackGround.h"

Game::Game()
{
	//当たり判定を有効化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
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
	NewGO<Sensor>(0, "sensor");
	NewGO<PlayerManagement>(0,"playerManagement");
	//m_stageModelRender.Init("Assets/modelData/stage1.tkm");
	//m_stageModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	//m_stageModelRender.SetRotation(Quaternion::Identity);
	//m_stageModelRender.SetScale(Vector3::One);
	//m_stageModelRender.Update();
	/*m_demobg.CreateFromModel(m_stageModelRender.GetModel(), m_stageModelRender.GetModel().GetWorldMatrix());*/

	LevelDesign();

	m_miniMap = NewGO<MiniMap>(0, "miniMap");

	return true;
}

void Game::LevelDesign()
{
	//レベルデザイン処理
	m_levelRender.Init("Assets/modelData/level/debug_1.tkl", [&](LevelObjectData& objData) {

		//名前がunityChanなら
		if (objData.ForwardMatchName(L"FootmanHP") == true) {
			//m_mirror = NewGO<Mirror>(0, "mirror");

			m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			m_enemyNormal->SetPosition(objData.position);
			//m_enemyNormal->SetRotation(Quaternion(objData.rotation.x, objData.rotation.z, objData.rotation.y, objData.rotation.w));
			m_enemyNormal->SetRotation(objData.rotation);
			m_enemyNormal->SetScale(objData.scale);

			return true;
		}

		//名前がbackgroundなら
		if (objData.EqualObjectName(L"debug") == true) {

			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}
		return true;
		}
	);
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
