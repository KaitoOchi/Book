#include "stdafx.h"
#include "Game.h"
#include"Player3D.h"
#include"Player2D.h"
#include"GameCamera.h"
#include "PlayerManagement.h"
#include "GameUI.h"
#include "Title.h"
#include "SenSor.h"
#include "MiniMap.h"
#include "Enemy_Normal.h"
#include "Enemy_Serch.h"
#include "Enemy_Charge.h"
#include "BackGround.h"
#include "LightSensor.h"
#include "Wall.h"
#include "Treasure.h"
#include "Gost.h"
Game::Game()
{
	//�����蔻���L����
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
	//NewGO<Sensor>(0, "sensor");
	
	NewGO<GameUI>(0, "gameUI");
	NewGO<LightSensor>(0, "lightSensor");
	//m_stageModelRender.Init("Assets/modelData/stage1.tkm");
	//m_stageModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	//m_stageModelRender.SetRotation(Quaternion::Identity);
	//m_stageModelRender.SetScale(Vector3::One);
	//m_stageModelRender.Update();
	/*m_demobg.CreateFromModel(m_stageModelRender.GetModel(), m_stageModelRender.GetModel().GetWorldMatrix());*/

	LevelDesign();
	m_playerManagement = NewGO<PlayerManagement>(0, "playerManagement");
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_pointLight.SetColor(Vector3(10.0f, 0.0f, 0.0f));
	m_pointLight.SetRange(100.0f);
	m_pointLight.Update();
	m_miniMap = NewGO<MiniMap>(0, "miniMap");
	return true;
}

void Game::LevelDesign()
{
	//���x���f�U�C������
	m_levelRender.Init("Assets/modelData/level/debug.tkl", [&](LevelObjectData& objData) {

		//���O��unityChan�Ȃ�
		if (objData.ForwardMatchName(L"FootmanHP") == true) {
			//m_mirror = NewGO<Mirror>(0, "mirror");

			//m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			//m_enemyNormal->SetPosition(objData.position);
			//m_enemyNormal->SetRotation(objData.rotation);
			//m_enemyNormal->SetScale(objData.scale);

			//// �p�X�ړ��̎w��
			//m_enemyNormal->Pass(0);

			m_enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
			m_enemyCharge->SetPosition(objData.position);
			m_enemyCharge->SetRotation(objData.rotation);
			m_enemyCharge->SetScale(objData.scale);

			return true;
		}

		//���O��background�Ȃ�
		if (objData.EqualObjectName(L"debug") == true) {

			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}
		/*if (objData.EqualObjectName(L"box") == true) {

			m_wall = NewGO<Wall>(0, "wall");
			m_wall ->SetPosition(objData.position);
			m_wall->SetRotation(objData.rotation);
			m_wall->SetScale(objData.scale);

			return true;
		}*/
		if (objData.EqualObjectName(L"unityChan") == true) {

			m_enemySerch = NewGO<Enemy_Serch>(0, "enemySerch");
			m_enemySerch->SetPosition(objData.position);
			m_enemySerch->SetRotation(objData.rotation);
			m_enemySerch->SetScale(objData.scale);

			return true;
		}
		if (objData.EqualObjectName(L"debugtoumei") == true) {

			//m_trans = NewGO<TransparentBox>(0, "transparentBox");
			//m_trans->SetPosition(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"item") == true) {

			m_treaSure = NewGO<Treasure>(0, "treaSure");
			m_treaSure->SetPosition(objData.position);
			m_treaSure->SetScale(objData.scale);
			m_treaSure->SetRotation(objData.rotation);
			return true;
		}
		if (objData.EqualObjectName(L"gost") == true) {

			m_gost = NewGO<Gost>(0, "gost");
			m_gost->SetPosition(objData.position);
			m_gost->SetScale(objData.scale);
			m_gost->SetRotation(objData.rotation);
			return true;
		}
		if (objData.EqualObjectName(L"clear") == true) {

			SetClearPosition(objData.position);

			m_pointLight.SetPosition(Vector3(m_position.x,m_position.y+10.0f,m_position.z));
			return true;
		}

		return true;
		}
	);
}

void Game::Update()
{
	Vector3 diff = m_playerManagement->GetPosition()- GetClearPosition();
	if (diff.LengthSq() <= 120.0f*120.0f)
	{
		m_gameState = m_enGameState_GameClear;
	}
		
	MnageState();
	m_pointLight.Update();
}
void Game::ClearState()
{
	//NewGO<Title>(0, "title");
	//DeleteGO(this);
	int a = 0;
}

void Game::MnageState()
{
	switch (m_gameState)
	{
	case Game::m_enGameState_DuringGamePlay:
		break;
	case Game::m_enGameState_GameClear:
		ClearState();
		break;
	case Game::m_enGameState_GameOver:
		break;
	case Game::m_enGameState_GameBuck:
		break;
	default:
		break;
	}
}
void Game::Render(RenderContext& rc)
{
	m_stageModelRender.Draw(rc);
}
