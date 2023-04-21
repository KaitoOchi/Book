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
#include "Enemy.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"
#include "Enemy_Clear.h"
#include "BackGround.h"
#include "LightSensor.h"
#include "Wall.h"
#include "Treasure.h"
#include "Ghost.h"
#include "FlashBom.h"
#include "SoundBom.h"
#include "Fade.h"
#include "Result.h"
Game::Game()
{
	//・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ阡ｻ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽL・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

Game::~Game()
{
	DeleteGO(m_player3D);
	DeleteGO(m_player2D);
	DeleteGO(m_playerManagement);
	//・ｽG・ｽl・ｽ~・ｽ[
	DeleteGO(m_enemyNormal);
	DeleteGO(m_enemyCharge);
	DeleteGO(m_enemySearch);
	DeleteGO(m_enemyClear);
	//オブジェクト
	//・ｽI・ｽu・ｽW・ｽF・ｽN・ｽg
	DeleteGO(FindGO<Sensor>("sensor"));
	DeleteGO(FindGO<GameUI>("gameUI"));
	DeleteGO(m_miniMap);
	DeleteGO(m_gamecamera);
	DeleteGO(m_backGround);
	//・ｽA・ｽC・ｽe・ｽ・ｽ
	DeleteGO(m_soundBom);
	DeleteGO(m_flahBom);
}

bool Game::Start()
{
	m_player2D=NewGO<Player2D>(0,"player2d");
	m_player3D = NewGO<Player3D>(0, "player3d");
	m_gamecamera=NewGO<GameCamera>(0, "gameCamera");
	NewGO<Sensor>(0, "sensor");
	m_playerManagement=NewGO<PlayerManagement>(0,"playerManagement");
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	NewGO<GameUI>(0, "gameUI");
	//NewGO<LightSensor>(0, "lightSensor");
	//m_stageModelRender.Init("Assets/modelData/stage1.tkm");
	//m_stageModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	//m_stageModelRender.SetRotation(Quaternion::Identity);
	//m_stageModelRender.SetScale(Vector3::One);
	//m_stageModelRender.Update();
	/*m_demobg.CreateFromModel(m_stageModelRender.GetModel(), m_stageModelRender.GetModel().GetWorldMatrix());*/
	m_flahBom = NewGO<FlashBom>(0, "flashBom");
	m_soundBom = NewGO<SoundBom>(0, "soundBom");
	m_pointLight[0].SetPointLight(
		0,
		Vector3::Zero,
		{ 10.0f, 0.0f, 0.0f },
		100.0f
	);

	m_pointLight[1].SetPointLight(
		1,
		Vector3::Zero,
		{ 00.0f, 10.0f, 0.0f },
		50.0f
	);

	m_pointLight[2].SetPointLight(
		2,
		Vector3::Zero,
		{ 0.0f, 0.0f, 10.0f },
		100.0f
	);

	m_pointLight[3].SetPointLight(
		3,
		Vector3::Zero,
		{ 10.0f, 0.0f, 0.0f },
		150.0f
	);

	LevelDesign();

	for (int i = 0; i < 4; i++) {
		m_pointLight[i].Update();
	}

	m_spotLight.SetSpotLight(
		0,
		{ 0.0f, 40.0f, 0.0f },
		{ 0.0f, 10.0f, 0.0f },
		400.0f,
		Vector3::AxisZ,
		45.0f
	);

	//m_miniMap = NewGO<MiniMap>(0, "miniMap");
	//・ｽt・ｽF・ｽ[・ｽh・ｽﾌ擾ｿｽ・ｽ・ｽ
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void Game::LevelDesign()
{
	// ・ｽ・ｽ・ｽx・ｽ・ｽ・ｽf・ｽU・ｽC・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	m_levelRender.Init("Assets/modelData/level/debug_1.tkl", [&](LevelObjectData& objData) {

		// ・ｽ・ｽ・ｽO・ｽ・ｽunityChan・ｽﾈゑｿｽ
		if (objData.ForwardMatchName(L"FootmanHP") == true) {
			//m_mirror = NewGO<Mirror>(0, "mirror");

			m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			m_enemyNormal->SetPosition(objData.position);
			m_enemyNormal->SetRotation(objData.rotation);
			m_enemyNormal->SetScale(objData.scale);
			// Enemy・ｽﾌ・ｿｽ・ｽX・ｽg・ｽﾉ追会ｿｽ
			m_enemyList.push_back(m_enemyNormal);

			// ・ｽp・ｽX・ｽﾚ難ｿｽ・ｽﾌ指・ｽ・ｽ
			m_enemyNormal->Pass(0);
			m_enemyClear = NewGO<Enemy_Clear>(0, "enemyClear");
			m_enemyClear->SetPosition(objData.position);
			m_enemyClear->SetRotation(objData.rotation);
			m_enemyClear->SetScale(objData.scale);
			// Enemyのリストに追加
			m_enemyList.push_back(m_enemyClear);

			// パス移動の指定
			m_enemyClear->Pass(7);
			return true;
		}

		//// ・ｽ・ｽ・ｽO・ｽ・ｽ EnemyNormal ・ｽﾈゑｿｽ
		//if (objData.ForwardMatchName(L"EnemyNormal") == true) {

		//	// ・ｽG・ｽl・ｽ~・ｽ[・ｽｶ撰ｿｽ
		//	m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
		//	// ・ｽ・ｽ・ｽg・ｽ・ｽ Normal ・ｽﾅゑｿｽ・ｽ・ｽﾆ具ｿｽ・ｽ・ｽ・ｽ・ｽ
		//	m_enemyNormal->m_enemyType = Enemy::Normal;
		//	// ・ｽ・ｽ・ｽW・ｽA・ｽ・ｽ]・ｽA・ｽX・ｽP・ｽ[・ｽ・ｽ・ｽﾌ設抵ｿｽ
		//	m_enemyNormal->SetPosition(objData.position);
		//	m_enemyNormal->SetRotation(objData.rotation);
		//	m_enemyNormal->SetScale(objData.scale);
		//	// ・ｽp・ｽX・ｽﾚ難ｿｽ・ｽﾌ指・ｽ・ｽ
		//	m_enemyNormal->Pass(0);
		//	// Enemy・ｽﾌ・ｿｽ・ｽX・ｽg・ｽﾉ追会ｿｽ
		//	m_enemyList.push_back(m_enemyNormal);
		//}

		//// ・ｽ・ｽ・ｽO・ｽ・ｽ EnemyCharge ・ｽﾈゑｿｽ
		//if (objData.ForwardMatchName(L"EnemyCharge") == true) {

		//	// ・ｽG・ｽl・ｽ~・ｽ[・ｽｶ撰ｿｽ
		//	m_enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
		//	// ・ｽ・ｽ・ｽg・ｽ・ｽ Charge ・ｽﾅゑｿｽ・ｽ・ｽﾆ具ｿｽ・ｽ・ｽ・ｽ・ｽ
		//	m_enemyCharge->m_enemyType = Enemy::Charge;
		//	// ・ｽ・ｽ・ｽW・ｽA・ｽ・ｽ]・ｽA・ｽX・ｽP・ｽ[・ｽ・ｽ・ｽﾌ設抵ｿｽ
		//	m_enemyCharge->SetPosition(objData.position);
		//	m_enemyCharge->SetRotation(objData.rotation);
		//	m_enemyCharge->SetScale(objData.scale);
		//	// ・ｽp・ｽX・ｽﾚ難ｿｽ・ｽﾌ指・ｽ・ｽ
		//	m_enemyCharge->Pass(7);
		//	// Enemy・ｽﾌ・ｿｽ・ｽX・ｽg・ｽﾉ追会ｿｽ
		//	m_enemyList.push_back(m_enemyCharge);
		//}

		//// ・ｽ・ｽ・ｽO・ｽ・ｽ EnemySearch ・ｽﾈゑｿｽ
		//if(objData.ForwardMatchName(L"EnemySearch") == true) {

		//	// ・ｽG・ｽl・ｽ~・ｽ[・ｽｶ撰ｿｽ
		//	m_enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
		//	// ・ｽ・ｽ・ｽg・ｽ・ｽ Charge ・ｽﾅゑｿｽ・ｽ・ｽﾆ具ｿｽ・ｽ・ｽ・ｽ・ｽ
		//	m_enemySearch->m_enemyType = Enemy::Search;
		//	// ・ｽ・ｽ・ｽW・ｽA・ｽ・ｽ]・ｽA・ｽX・ｽP・ｽ[・ｽ・ｽ・ｽﾌ設抵ｿｽ
		//	m_enemySearch->SetPosition(objData.position);
		//	m_enemySearch->SetRotation(objData.rotation);
		//	m_enemySearch->SetScale(objData.scale);
		//	// Enemy・ｽﾌ・ｿｽ・ｽX・ｽg・ｽﾉ追会ｿｽ
		//	m_enemyList.push_back(m_enemySearch);
		//}

		//・ｽ・ｽ・ｽO・ｽ・ｽbackground・ｽﾈゑｿｽ
		if (objData.EqualObjectName(L"debug") == true) {

			// ・ｽw・ｽi・ｽｶ撰ｿｽ
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}
		// ・ｽ・ｽ・ｽO・ｽ・ｽbox・ｽﾌとゑｿｽ
		if (objData.EqualObjectName(L"box") == true) {

			m_wall = NewGO<Wall>(0, "wall");
			m_wall ->SetPosition(objData.position);
			m_wall->SetRotation(objData.rotation);
			m_wall->SetScale(objData.scale);

			return true;
		}
		if (objData.EqualObjectName(L"unityChan") == true) {

			m_enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
			m_enemySearch->SetPosition(objData.position);
			m_enemySearch->SetRotation(objData.rotation);
			m_enemySearch->SetScale(objData.scale);
			// Enemy・ｽﾌ・ｿｽ・ｽX・ｽg・ｽﾉ追会ｿｽ
			m_enemyList.push_back(m_enemySearch);

			return true;
		}
		if (objData.EqualObjectName(L"debugtoumei") == true) {

			m_player3D->m_ghostpositions.push_back(objData.position);
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

			m_ghost = NewGO<Ghost>(0, "ghost");
			m_ghost->SetPosition(objData.position);
			m_ghost->SetScale(objData.scale);
			m_ghost->SetRotation(objData.rotation);
			return true;
		}
		if (objData.EqualObjectName(L"clear") == true) {

			SetClearPosition(objData.position);

			for (int i = 0; i < 4; i++) {
				m_pointLight[i].SetPosition(Vector3(m_position.x + (i * 100), m_position.y + 10.0f, m_position.z));
			}
			return true;
		}

		return true;
		}
	);
}

void Game::Update()
{
	Vector3 diff = m_playerManagement->GetPosition()- GetClearPosition();
	if (diff.LengthSq() <= 120.0f*120.0f&&m_gameState==m_enGameState_GameClearable)
	{
		m_gameState = m_enGameState_GameClear;
	}
		
	MnageState();

	for (int i = 0; i < 4; i++) {
		m_pointLight[i].Update();
	}

	m_spotLight.Update();
}
void Game::ClearState()
{
	NewGO<Result>(0, "result");
	DeleteGO(this);

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
