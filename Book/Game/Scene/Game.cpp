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
#include "Star.h"
#include <random>
#include"Gage.h"
#include "Star.h"
Game::Game()
{
	//・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ阡ｻ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽL・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

Game::~Game()
{
	DeleteGO(m_player3D);
	DeleteGO(m_player2D);
	DeleteGO(m_playerManagement);
	//�E�G�E�l�E�~�E�[
	DeleteGO(m_enemyNormal);
	DeleteGO(m_enemyCharge);
	DeleteGO(m_enemySearch);
	DeleteGO(m_enemyClear);
	//�I�u�W�F�N�g
	//�E�I�E�u�E�W�E�F�E�N�E�g
	DeleteGO(FindGO<Sensor>("sensor"));
	DeleteGO(FindGO<GameUI>("gameUI"));
	DeleteGO(m_miniMap);
	DeleteGO(m_gamecamera);
	DeleteGO(m_backGround);
	//�E�A�E�C�E�e�E��E�
	DeleteGO(m_soundBom);
	DeleteGO(m_flahBom);
}

bool Game::Start()
{
	m_player2D=NewGO<Player2D>(0,"player2d");
	m_player3D = NewGO<Player3D>(0, "player3d");
	m_gamecamera=NewGO<GameCamera>(0, "gameCamera");
	NewGO<Sensor>(0, "sensor");
	m_playerManagement = NewGO<PlayerManagement>(0, "playerManagement");
	m_playerManagement->SetPlayer2DAND3D(m_player3D, m_player2D);
	NewGO<GameUI>(0, "gameUI");
	//NewGO<Gage>(0,"gage");
	
	
	//NewGO<LightSensor>(0, "lightSensor");
	//m_stageModelRender.Init("Assets/modelData/stage1.tkm");
	//m_stageModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	//m_stageModelRender.SetRotation(Quaternion::Identity);
	//m_stageModelRender.SetScale(Vector3::One);
	//m_stageModelRender.Update();
	/*m_demobg.CreateFromModel(m_stageModelRender.GetModel(), m_stageModelRender.GetModel().GetWorldMatrix());*/

	//m_modelRender.Init("Assets/modelData/wall1.tkm");

	m_flahBom = NewGO<FlashBom>(0, "flashBom");
	m_soundBom = NewGO<SoundBom>(0, "soundBom");

	m_pointLight[0].SetPointLight(
		0,
		Vector3::Zero,
		{ 0.0f, 0.0f, 0.0f },
		100.0f
	);

	m_pointLight[1].SetPointLight(
		1,
		Vector3::Zero,
		{ 0.0f, 0.0f, 0.0f },
		100.0f
	);

	m_pointLight[2].SetPointLight(
		2,
		Vector3::Zero,
		{ 0.0f, 0.0f, 0.0f },
		100.0f
	);

	m_pointLight[3].SetPointLight(
		3,
		Vector3::Zero,
		{ 0.0f, 50.0f, 0.0f },
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
	//�E�t�E�F�E�[�E�h�E�̏��E��E�
	//m_fade = FindGO<Fade>("fade");
	//m_fade->StartFadeIn();
	//for (int i = 0; i <= m_enemyList.size(); i++)
	//{
	//	m_star = NewGO<Star>(0, "star");
	//	m_starList.push_back(m_star);
	//}

	//�����_���Ȓl�𐶐�����
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int>dist(0, 3);
	m_lightNumber = dist(mt);
	m_position = m_pointLight[m_lightNumber].GetPosition();
	return true;
}

void Game::LevelDesign()
{
	// レベルデザイン処理
	m_levelRender.Init("Assets/modelData/level_test/level_test.tkl", [&](LevelObjectData& objData)
/*	m_levelRender.Init("Assets/modelData/level/debug.tkl", [&](LevelObjectData& objData)*/ {
		// �E��E��E�O�E��E�unityChan�E�Ȃ�

		//if (objData.ForwardMatchName(L"FootmanHP") == true) {
		//	//m_mirror = NewGO<Mirror>(0, "mirror");

		//	// エネミーを生成
		//	m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
		//	// 自身の属性を教える
		//	m_enemyNormal->m_enemyType = Enemy::Normal;
		//	// 座標・回転・スケールを教える
		//	m_enemyNormal->SetPosition(objData.position);
		//	m_enemyNormal->SetRotation(objData.rotation);
		//	m_enemyNormal->SetScale(objData.scale);
		//	// パス移動の順路を指定
		//	m_enemyNormal->Pass(0);
		//	// エネミーのリストに追加する
		//	m_enemyList.push_back(m_enemyNormal);

		//	m_enemyClear = NewGO<Enemy_Clear>(0, "enemyClear");
		//	m_enemyClear->SetPosition(objData.position);
		//	m_enemyClear->SetRotation(objData.rotation);
		//	m_enemyClear->SetScale(objData.scale);
		//	// Enemy�̃��X�g�ɒǉ�
		//	m_enemyList.push_back(m_enemyClear);

		//	// �p�X�ړ��̎w��
		//	m_enemyClear->Pass(7);

		//	return true;
		//}

		// 名前が Normal のとき
		if (objData.EqualObjectName(L"Normal") == true) {
			// エネミーを生成
			m_enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			// 自身の属性を教える
			m_enemyNormal->m_enemyType = Enemy::Normal;
			// 座標・回転・スケールを教える
			m_enemyNormal->SetPosition(objData.position);
			m_enemyNormal->SetRotation(objData.rotation);
			m_enemyNormal->SetScale(objData.scale);
			// パス移動の順路を指定
			m_enemyNormal->Pass(0);
			// スポットライトの数を加算
			m_enemyNormal->AddSpotLightNum();
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
			// パス移動の順路を指定
			m_enemyCharge->Pass(0);
			// スポットライトの数を加算
			m_enemyCharge->AddSpotLightNum();
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
			// スポットライトの数を加算
			m_enemySearch->AddSpotLightNum();
			// エネミーのリストに追加する
			m_enemyList.push_back(m_enemySearch);
			return true;
		}

		// ステージのレベル
		{
			//名前がbackgroundなら
			if (objData.EqualObjectName(L"base") == true)
		/*	if (objData.EqualObjectName(L"debug") == true)*/ {
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
				m_wall = NewGO<Wall>(0, "wall");
				m_wall->SetPosition(objData.position);
				m_wall->SetRotation(objData.rotation);
				m_wall->SetScale(objData.scale);

				return true;
			}
			//// 名前がgapのとき
			//if (objData.EqualObjectName(L"gap") == true) {
			//	// 隙間を生成する
			//	m_wall = NewGO<Wall>(0, "wall");
			//	//m_wall->SetWallModel(1);
			//	m_wall->SetPosition(objData.position);
			//	m_wall->SetRotation(objData.rotation);
			//	m_wall->SetScale(objData.scale);

			//	return true;
			//}
			//// 名前がdecorationのとき
			//if (objData.EqualObjectName(L"decoration") == true) {
			//	// 障害物を生成
			//	m_wall = NewGO<Wall>(0, "wall");
			//	//m_wall->SetWallModel(0);
			//	m_wall->SetPosition(objData.position);
			//	m_wall->SetRotation(objData.rotation);
			//	m_wall->SetScale(objData.scale);

			//	return true;
			//}
			//// 名前がstartのとき
			//if (objData.EqualObjectName(L"start") == true) {
			//	// スタートを生成
			//	m_wall = NewGO<Wall>(0, "wall");
			//	//m_wall->SetWallModel(0);
			//	m_wall->SetPosition(objData.position);
			//	m_wall->SetRotation(objData.rotation);
			//	m_wall->SetScale(objData.scale);

			//	return true;
			//}
			//// 名前がgoalのとき
			//if (objData.EqualObjectName(L"goal") == true) {
			//	// ゴールを生成
			//	m_wall = NewGO<Wall>(0, "wall");
			//	//m_wall->SetWallModel(0);
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

		if (objData.EqualObjectName(L"debugtoumei") == true) {

			m_player3D->m_ghostpositions.push_back(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"otakara") == true) {

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
	if (m_gameState==m_enGameState_GameClearable)
	{
		Clearable();
	}
		
	MnageState();

	for (int i = 0; i < 4; i++) {
		m_pointLight[i].Update();
	}
}
void Game::Clearable()
{
	Vector3 diff = m_playerManagement->GetPosition() - m_pointLight[m_lightNumber].GetPosition();
	if (diff.LengthSq() <= 120.0f * 120.0f)
	{
		m_gameState = m_enGameState_GameClear;
	}
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
