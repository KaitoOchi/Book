#include "stdafx.h"
#include "Game.h"
#include"Player3D.h"
#include"Player2D.h"
#include"GameCamera.h"
#include "PlayerManagement.h"
#include "Title.h"
#include "SenSor.h"
#include "MiniMap.h"
#include "Enemy.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"
#include "BackGround.h"

Game::Game()
{
	//当たり判定を有効化
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
	m_levelRender.Init("Assets/modelData/level/debug_2.tkl", [&](LevelObjectData& objData) {

		//名前がunityChanなら
		if (objData.ForwardMatchName(L"FootmanHP") == true) {
			//m_mirror = NewGO<Mirror>(0, "mirror");

			//m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			//m_enemyNormal->SetPosition(objData.position);
			//m_enemyNormal->SetRotation(objData.rotation);
			//m_enemyNormal->SetScale(objData.scale);

			//// パス移動の指定
			//m_enemyNormal->Pass(0);

			m_enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
			m_enemyCharge->SetPosition(objData.position);
			m_enemyCharge->SetRotation(objData.rotation);
			m_enemyCharge->SetScale(objData.scale);

			// パス移動の指定
			m_enemyCharge->Pass(7);

			return true;
		}

		// 名前が EnemyNormal なら
		if (objData.ForwardMatchName(L"EnemyNormal") == true) {

			// エネミーを生成
			m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			// 自身が Normal であると教える
			m_enemyNormal->m_enemyType = Enemy::Normal;
			// 座標、回転、スケールの設定
			m_enemyNormal->SetPosition(objData.position);
			m_enemyNormal->SetRotation(objData.rotation);
			m_enemyNormal->SetScale(objData.scale);
			// パス移動の指定
			m_enemyNormal->Pass(0);
			// Enemyのリストに追加
			m_enemyList.push_back(m_enemyNormal);
		}

		// 名前が EnemyCharge なら
		if (objData.ForwardMatchName(L"EnemyCharge") == true) {

			// エネミーを生成
			m_enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
			// 自身が Charge であると教える
			m_enemyCharge->m_enemyType = Enemy::Charge;
			// 座標、回転、スケールの設定
			m_enemyCharge->SetPosition(objData.position);
			m_enemyCharge->SetRotation(objData.rotation);
			m_enemyCharge->SetScale(objData.scale);
			// パス移動の指定
			m_enemyCharge->Pass(7);
			// Enemyのリストに追加
			m_enemyList.push_back(m_enemyCharge);
		}

		// 名前が EnemySearch なら
		if(objData.ForwardMatchName(L"EnemySearch") == true) {

			// エネミーを生成
			m_enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
			// 自身が Charge であると教える
			m_enemySearch->m_enemyType = Enemy::Search;
			// 座標、回転、スケールの設定
			m_enemySearch->SetPosition(objData.position);
			m_enemySearch->SetRotation(objData.rotation);
			m_enemySearch->SetScale(objData.scale);
			// Enemyのリストに追加
			m_enemyList.push_back(m_enemyCharge);
		}

		//名前がbackgroundなら
		if (objData.EqualObjectName(L"debug") == true) {

			// 背景を生成
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}

		if (objData.EqualObjectName(L"unityChan") == true) {

			m_enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
			m_enemySearch->SetPosition(objData.position);
			m_enemySearch->SetRotation(objData.rotation);
			m_enemySearch->SetScale(objData.scale);

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
