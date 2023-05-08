#include "stdafx.h"
#include "Game.h"
#include"Player3D.h"
#include"Player2D.h"
#include"GameCamera.h"
#include "PlayerManagement.h"
#include "GameUI.h"
#include "Title.h"
#include "Sensor.h"
#include "MiniMap.h"
#include "Enemy.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"
#include "Enemy_Clear.h"
#include "BackGround.h"
#include "Stage/Wall/Wall.h"
#include "Stage/Wall/Wall_Decoration.h"
#include "Stage/Wall/Wall_Door.h"
#include "Stage/Wall/Wall_Gap.h"
#include "Stage/Wall/Wall_Normal.h"
#include "Stage/Wall/Wall_Post.h"
#include "Treasure.h"
#include "Ghost.h"
#include "GhostBox.h"
#include "PhysicsGhost.h"
#include "FlashBom.h"
#include "SoundBom.h"
#include "Fade.h"
#include "Result.h"
#include "Star.h"
#include <random>
#include"Gage.h"
#include "Star.h"
#include "Pause.h"
#include "CountDown.h"
#include "SecurityCamera.h"
#include "nature/SkyCube.h"


Game::Game()
{
	//・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ阡ｻ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽL・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

Game::~Game()
{

	//�E�G�E�l�E�~�E�[
	for (int i = 0; i < m_enemyList.size(); i++)
	{
		DeleteGO(m_enemyList[i]);
	}
	//�I�u�W�F�N�g
	//�E�I�E�u�E�W�E�F�E�N�E�g
	DeleteGO(FindGO<Sensor>("sensor"));
	DeleteGO(m_gameUI);
	DeleteGO(FindGO<Gage>("gage"));
	DeleteGO(m_miniMap);
	DeleteGO(m_gamecamera);
	//壁や床の削除
	DeleteGO(m_backGround);
	for (int i = 0; i < m_wallList.size(); i++)
	{
		DeleteGO(m_wallList[i]);
	}
	//�E�A�E�C�E�e�E��E�
	DeleteGO(m_soundBom);
	DeleteGO(m_flahBom);
	DeleteGO(m_treaSure);
	for (int i = 0; i < m_sensorList.size(); i++)
	{
		DeleteGO(m_sensorList[i]);
	}

	for (int i = 0; i < m_SecurityCameraList.size(); i++)
	{
		DeleteGO(m_SecurityCameraList[i]);
	}

	DeleteGO(m_player3D);
	DeleteGO(m_player2D);
	DeleteGO(m_playerManagement);

	DeleteGO(FindGO<CountDown>("countDown"));

	// ライトの数を0に
	RenderingEngine::GetInstance()->GetLightCB().ptNum = 0;
	RenderingEngine::GetInstance()->GetLightCB().spNum = 0;
}

void Game::GameDelete(const int nextScene)
{		
	m_nextScene = nextScene;
	m_isWaitFadeOut = true;	
	m_fade->StartFadeOut();
	GameManager::GetInstance()->DeleteBGM();
}
	
void Game::GamePos()
{
	switch (m_nextScene) {
	case 1:
		//リトライ画面へ移行
		NewGO<Game>(0, "game");
		break;
	case 2:
		//タイトル画面へ移行
		NewGO<Title>(0, "title");
		break;
	default:
		break;
	}
}

bool Game::Start()
{
	m_player2D=NewGO<Player2D>(0,"player2d");
	m_player3D = NewGO<Player3D>(0, "player3d");
	m_gamecamera=NewGO<GameCamera>(0, "gameCamera");
	m_playerManagement = NewGO<PlayerManagement>(0, "playerManagement");
	m_playerManagement->SetPlayer2DAND3D(m_player3D, m_player2D);
	m_flahBom = NewGO<FlashBom>(0, "flashBom");
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	NewGO<Gage>(0,"gage");
	NewGO<CountDown>(0, "countDown");

	//NewGO<SecurityCamera>(0, "securityCamera");

	NewGO<Pause>(0, "pause");
	NewGO<SkyCube>(0, "skyCube");


	//m_stageModelRender.Init("Assets/modelData/stage1.tkm");
	//m_stageModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	//m_stageModelRender.SetRotation(Quaternion::Identity);
	//m_stageModelRender.SetScale(Vector3::One);
	//m_stageModelRender.Update();
	/*m_demobg.CreateFromModel(m_stageModelRender.GetModel(), m_stageModelRender.GetModel().GetWorldMatrix());*/

	//m_modelRender.Init("Assets/modelData/wall1.tkm");

	m_soundBom = NewGO<SoundBom>(0, "soundBom");

	m_pointLight[0].SetPointLight(
		0,
		Vector3::Zero,
		{ 0.0f, 0.0f, 0.0f },
		200.0f
	);

	m_pointLight[1].SetPointLight(
		1,
		Vector3::Zero,
		{ 0.0f, 0.0f, 0.0f },
		200.0f
	);

	m_pointLight[2].SetPointLight(
		2,
		Vector3::Zero,
		{ 0.0f, 0.0f, 0.0f },
		200.0f
	);


	LevelDesign();
	//お宝を作成する
	m_treaSure = NewGO<Treasure>(0, "treaSure");

	for (int i = 0; i < 3; i++) {
		m_pointLight[i].Update();
	}

	m_miniMap = NewGO<MiniMap>(0, "miniMap");
	//�E�t�E�F�E�[�E�h�E�̏��E��E�
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	
	
	for (int i = 0; i <= m_enemyList.size(); i++)
	{
		m_star = NewGO<Star>(0, "star");
		m_starList.push_back(m_star);
	}

	//�����_���Ȓl�𐶐�����
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int>dist(0, 3);
	m_lightNumber = dist(mt);
	m_position = m_pointLight[m_lightNumber].GetPosition();


	GameManager::GetInstance()->SetGameState(GameManager::GetInstance()->enState_Game);

	GameManager::GetInstance()->SetBGM(21);
	return true;
}

void Game::LevelDesign()
{
	// レベルデザイン処理
	m_levelRender.Init("Assets/modelData/level_test/tkl/level.tkl", [&](LevelObjectData& objData){

		// 名前が Normal のとき
		if (objData.EqualObjectName(L"Normal") == true) {
			 //エネミーを生成
			m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			// 自身の属性を教える
			m_enemyNormal->m_enemyType = Enemy::Normal;
			// 座標・回転・スケールを教える
			m_enemyNormal->SetPosition(objData.position);
			m_enemyNormal->SetRotation(objData.rotation);
			m_enemyNormal->SetScale(objData.scale);

			m_enemyNormal->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			m_enemyNormal->Pass(0);
			// エネミーのリストに追加する
			m_enemyList.push_back(m_enemyNormal);
			return true;
		}

		// 名前が Charge のとき
		if (objData.EqualObjectName(L"Charge") == true) {
			// エネミーを生成
			m_enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
			// 自身の属性を教える
			m_enemyCharge->m_enemyType = Enemy::Charge;
			// 座標・回転・スケールを教える
			m_enemyCharge->SetPosition(objData.position);
			m_enemyCharge->SetRotation(objData.rotation);
			m_enemyCharge->SetScale(objData.scale);

			m_enemyCharge->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			m_enemyCharge->Pass(1);
			// エネミーのリストに追加する
			m_enemyList.push_back(m_enemyCharge);
			return true;
		}

		// 名前が Search のとき
		if (objData.EqualObjectName(L"Search") == true) {
			// エネミーを生成
			m_enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
			// 自身の属性を教える
			m_enemySearch->m_enemyType = Enemy::Search;
			// 座標・回転・スケールを教える
			m_enemySearch->SetPosition(objData.position);
			m_enemySearch->SetRotation(objData.rotation);
			m_enemySearch->SetScale(objData.scale);

			m_enemySearch->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// エネミーのリストに追加する
			m_enemyList.push_back(m_enemySearch);
			return true;
		}

		// 名前が Clear のとき
		if (objData.EqualObjectName(L"Clear") == true) {
			// エネミーを生成
			m_enemyClear = NewGO<Enemy_Clear>(0, "enemyClear");
			// 自身の属性を教える
			m_enemyClear->m_enemyType = Enemy::Clear;
			// 座標・回転・スケールを教える
			m_enemyClear->SetPosition(objData.position);
			m_enemyClear->SetRotation(objData.rotation);
			m_enemyClear->SetScale(objData.scale);

			m_enemyClear->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			m_enemyClear->Pass(3);
			// エネミーのリストに追加する
			m_enemyList.push_back(m_enemyClear);
			return true;
		}

		// ステージのレベル
		{
			//名前がbackgroundなら
			if (objData.EqualObjectName(L"base") == true)
			/*if (objData.EqualObjectName(L"debug") == true)*/ {
				// 背景を生成
				m_backGround = NewGO<BackGround>(0, "backGround");
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);

				return true;
			}

			// 名前がboxなら
			if (objData.EqualObjectName(L"box") == true) {
				// 壁を生成
				m_normal = NewGO<Wall_Normal>(0, "wall_Normal");
				m_normal->SetPosition(objData.position);
				m_normal->SetRotation(objData.rotation);
				m_normal->SetScale(objData.scale);
				m_wallList.emplace_back(m_normal);
				return true;
			}

			// 名前がgapのとき
			if (objData.EqualObjectName(L"gap_1") == true) {
				// 隙間を生成する
				m_gap = NewGO<Wall_Gap>(0, "wall_Gap");
				m_gap->ModelLoad(1);
				m_gap->SetPosition(objData.position);
				m_gap->SetRotation(objData.rotation);
				m_gap->SetScale(objData.scale);
				m_wallList.emplace_back(m_gap);
				return true;
			}

			// 名前がgapのとき
			if (objData.EqualObjectName(L"gap_2") == true) {
				// 隙間を生成する
				m_gap = NewGO<Wall_Gap>(0, "wall_Gap");
				m_gap->ModelLoad(2);
				m_gap->SetPosition(objData.position);
				m_gap->SetRotation(objData.rotation);
				m_gap->SetScale(objData.scale);
				m_wallList.emplace_back(m_gap);
				return true;
			}

			// 名前がgapのとき
			if (objData.EqualObjectName(L"gap_3") == true) {
				// 隙間を生成する
				m_gap = NewGO<Wall_Gap>(0, "wall_Gap");
				m_gap->ModelLoad(3);
				m_gap->SetPosition(objData.position);
				m_gap->SetRotation(objData.rotation);
				m_gap->SetScale(objData.scale);
				m_wallList.emplace_back(m_gap);
				return true;
			}

			// 名前がpostのとき
			if (objData.EqualObjectName(L"post") == true) {
				// 隙間を生成する
				m_post = NewGO<Wall_Post>(0, "wall_Post");
				m_post->SetPosition(objData.position);
				m_post->SetRotation(objData.rotation);
				m_post->SetScale(objData.scale);
				m_wallList.emplace_back(m_post);
				return true;
			}

			// 名前がdecorationのとき
			if (objData.EqualObjectName(L"decoration") == true) {
				// 障害物を生成
				m_decoration = NewGO<Wall_Decoration>(0, "wall_Decoration");
				m_decoration->SetPosition(objData.position);
				m_decoration->SetRotation(objData.rotation);
				m_decoration->SetScale(objData.scale);
				m_wallList.emplace_back(m_decoration);
				return true;
			}

			// 名前がstartのとき
			if (objData.EqualObjectName(L"start") == true) {
				// スタートを生成
				m_door = NewGO<Wall_Door>(0, "wall_Door");
				m_door->SetPosition(objData.position);
				m_door->SetRotation(objData.rotation);
				m_door->SetScale(objData.scale);
				m_wallList.emplace_back(m_door);
				return true;
			}

			//// 名前がgoalのとき
			//if (objData.EqualObjectName(L"goal") == true) {
			//	// ゴールを生成
			//	m_wall = NewGO<Wall>(0, "wall");
			//	m_wall->SetPosition(objData.position);
			//	m_wall->SetRotation(objData.rotation);
			//	m_wall->SetScale(objData.scale);

			//	return true;
			//}
		}


		//if (objData.EqualObjectName(L"unityChan") == true) {

		//	m_enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
		//	m_enemySearch->SetPosition(objData.position);
		//	m_enemySearch->SetRotation(objData.rotation);
		//	m_enemySearch->SetScale(objData.scale);
		//	// Enemy・ｽﾌ・ｿｽ・ｽX・ｽg・ｽﾉ追会ｿｽ
		//	m_enemyList.push_back(m_enemySearch);

		//	return true;
		//}

		if (objData.EqualObjectName(L"sensor")==true) {
			m_sensor = NewGO<Sensor>(0, "sensor");
			m_sensor->SetPosition(objData.position);
			m_sensor->SetScale(objData.scale);
			m_sensor->SetRotation(objData.rotation);
			m_sensorList.emplace_back(m_sensor);

			return true;
		}

		if (objData.EqualObjectName(L"camera") == true) {
			m_securityCamera = NewGO<SecurityCamera>(0, "securityCamera");
			m_securityCamera->SetPosition(objData.position);
			m_securityCamera->SetType(0);
			m_SecurityCameraList.emplace_back(m_securityCamera);
			return true;
		}

		//if (objData.EqualObjectName(L"debugtoumei") == true) {
		if (objData.EqualObjectName(L"push") == true) {
			m_player3D->m_ghostpositions.push_back(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"otakara") == true) {

			m_treasurePositions.push_back(objData.position);
			
			return true;
		}
		if (objData.EqualObjectName(L"ghost") == true) {

			m_physicsGhost = NewGO<PhysicsGhost>(0, "physicsGhost");
			m_physicsGhost->SetPosition(objData.position);
			m_physicsGhost->SetScale(objData.scale);
			m_physicsGhost->SetRotation(objData.rotation);
			return true;
		}
		if (objData.EqualObjectName(L"physics") == true) {
			m_ghostBox = NewGO<GhostBox>(0,"ghostBox");
			m_ghostBox->SetPosition(objData.position);
			m_ghostBox->SetScale(objData.scale);
			m_ghostBox->SetRotation(objData.rotation);
		}
		if (objData.EqualObjectName(L"clear") == true) {

			SetClearPosition(objData.position);
			m_pointLight[lights].SetPosition(Vector3(m_position.x , m_position.y + 10.0f, m_position.z));
			lights++;
			return true;
		}
		return true;
		}
	);

	RenderingEngine::GetInstance()->GetLightCB().spNum = m_spotLigNum;
}

void Game::Update()
{		
	MnageState();

	for (int i = 0; i < 3; i++) {
		m_pointLight[i].Update();
	}

	//フェードの待機時間
	if (m_isWaitFadeOut) {
		//フェードし終えたら
		if (!m_fade->IsFade()) {

			if (m_gameState != m_enGameState_GameFade)
			{
				GamePos();
				
			}
			else
			{
				Result* result = NewGO<Result>(0, "result");
				result->SetTime(m_gameUI->GetTime());
			}
			DeleteGO(FindGO<Pause>("pause"));
			DeleteGO(this);
			GameManager::GetInstance()->DeleteBGM();
		}
	}
	

}
void Game::Clearable()
{
	
	Vector3 diff = m_playerManagement->GetPosition() - m_pointLight[m_lightNumber].GetPosition();
	if (diff.LengthSq() <= 120.0f * 120.0f)
	{
		GameDelete(0);
		m_gameState = m_enGameState_GameFade;
	
	}
	if (m_gameUI->GetTime() <= 0.0f)
	{
		m_gameState = m_enGameState_GameFade;
	}
	
}

void Game::ClearState()
{
	
}

void Game::MnageState()
{
	switch (m_gameState)
	{
	case Game::m_enGameState_GameClearable:
		Clearable();
		break;
	case Game::m_enGameState_DuringGamePlay:
		break;
	case Game::m_enGameState_GameFade:
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