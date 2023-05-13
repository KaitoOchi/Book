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
#include <random>
#include"Gage.h"
#include "Pause.h"
#include "CountDown.h"
#include "SecurityCamera.h"
#include "Event.h"
#include "nature/SkyCube.h"
#include "Wipe.h"


Game::Game()
{
	//・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ阡ｻ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽL・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	// はてなマークのエフェクト
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/effect/e/question/hatena.efk");
	//ビックリマーク
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/e/exclamation/exmark.efk");
	//星エフェクト
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/e/star/star.efk");
	//音と煙のエフェクト
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/e/otokemuri/otokemuri.efk");
	//煙のエフェクト
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/e/kemuri/kemuri.efk");

	//フレームレートを固定
	//g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Fix, GameManager::GetInstance()->GetFrameRate());
}

Game::~Game()
{

	//エネミーリストの解放
	for (int i = 0; i < m_enemyList.size(); i++)
	{
		DeleteGO(m_enemyList[i]);
	}
	m_enemyList.shrink_to_fit();

	//�I�u�W�F�N�g
	//�E�I�E�u�E�W�E�F�E�N�E�g
	DeleteGO(FindGO<Sensor>("sensor"));
	DeleteGO(m_gameUI);
	DeleteGO(m_gage);
	DeleteGO(m_miniMap);
	DeleteGO(m_pause);
	DeleteGO(m_gamecamera);

	//壁や床の削除
	DeleteGO(FindGO<BackGround>("backGround"));
	for (int i = 0; i < m_wallList.size(); i++)
	{
		DeleteGO(m_wallList[i]);
	}
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

	// エネミーの削除
	for (int i = 0; i < m_enemyList.size(); i++) {
		DeleteGO(m_enemyList[i]);
	}

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
	//環境光を初期化する
	RenderingEngine::GetInstance()->SetDirectionLight(Vector3(1, -1, -1), Vector3(0.01f, 0.01f, 0.01f));
	RenderingEngine::GetInstance()->SetAmbient(0.0f);
	//RenderingEngine::GetInstance()->Setm_directionLig(Vector3(1, -1, 1),0.0f, Vector3(0.2f, 0.2f, 0.2f));

	//スタート時を知らせる
	NotifyGameStart();

	//リストの初期化
	m_enemyList.clear();
	m_wallList.clear();
	m_sensorList.clear();
	m_SecurityCameraList.clear();


	m_player2D =NewGO<Player2D>(0,"player2d");
	m_player3D = NewGO<Player3D>(0, "player3d");
	m_playerManagement = NewGO<PlayerManagement>(0, "playerManagement");
	m_playerManagement->SetPlayer2DAND3D(m_player3D, m_player2D);

	m_gamecamera = NewGO<GameCamera>(0, "gameCamera");

	//UIの初期化
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	m_gage = NewGO<Gage>(0,"gage");
	m_miniMap = NewGO<MiniMap>(0, "miniMap");
	m_pause = NewGO<Pause>(0, "pause");
		
	NewGO<CountDown>(0, "countDown");
	NewGO<SkyCube>(0, "skyCube");
	NewGO<Wipe>(0, "wipe");

	RenderingEngine::GetInstance()->GetLightCB().ptNum = 3;
	LevelDesign();
	//お宝を作成する
	m_treaSure = NewGO<Treasure>(0, "treaSure");

	for (int i = 0; i < 3; i++) {
		m_pointLight[i].Update();
	}

	//�����_���Ȓl�𐶐�����
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int>dist(0, 3);
	m_lightNumber = dist(mt);
	m_position = m_pointLight[m_lightNumber].GetPosition();


	GameManager::GetInstance()->SetGameState(GameManager::enState_Game);
	GameManager::GetInstance()->SetBGM(21);

	//フェードの処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Game::LevelDesign()
{
	// レベルデザイン処理
	m_levelRender.Init("Assets/level3D/level0_1.tkl", [&](LevelObjectData& objData){
		/*
		// 名前が Normal のとき
		if (objData.EqualObjectName(L"Normal") == true) {
			 //エネミーを生成
			Enemy_Normal* enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			// 自身の属性を教える
			enemyNormal->m_enemyType = Enemy::Normal;
			// 座標・回転・スケールを教える
			enemyNormal->SetPosition(objData.position);
			enemyNormal->SetRotation(objData.rotation);
			enemyNormal->SetScale(objData.scale);

			enemyNormal->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			enemyNormal->Pass(0);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemyNormal);
			return true;
		}

		// 名前が Charge のとき
		if (objData.EqualObjectName(L"Charge") == true) {
			// エネミーを生成
			Enemy_Charge* enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
			// 自身の属性を教える
			enemyCharge->m_enemyType = Enemy::Charge;
			// 座標・回転・スケールを教える
			enemyCharge->SetPosition(objData.position);
			enemyCharge->SetRotation(objData.rotation);
			enemyCharge->SetScale(objData.scale);

			enemyCharge->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			enemyCharge->Pass(1);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemyCharge);
			return true;
		}

		// 名前が Search のとき
		if (objData.EqualObjectName(L"Search") == true) {
			// エネミーを生成
			Enemy_Search* enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
			// 自身の属性を教える
			enemySearch->m_enemyType = Enemy::Search;
			// 座標・回転・スケールを教える
			enemySearch->SetPosition(objData.position);
			enemySearch->SetRotation(objData.rotation);
			enemySearch->SetScale(objData.scale);

			enemySearch->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// エネミーのリストに追加する
			m_enemyList.push_back(enemySearch);
			return true;
		}

		// 名前が Clear のとき
		if (objData.EqualObjectName(L"Clear") == true) {
			// エネミーを生成
			Enemy_Clear* enemyClear = NewGO<Enemy_Clear>(0, "enemyClear");
			// 自身の属性を教える
			enemyClear->m_enemyType = Enemy::Clear;
			// 座標・回転・スケールを教える
			enemyClear->SetPosition(objData.position);
			enemyClear->SetRotation(objData.rotation);
			enemyClear->SetScale(objData.scale);

			enemyClear->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			enemyClear->Pass(2);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemyClear);
			return true;
		}
		*/

		// ステージのレベル
		{
			//名前がbackgroundなら
			if (objData.EqualObjectName(L"base") == true)
			/*if (objData.EqualObjectName(L"debug") == true)*/ {
				// 背景を生成
				BackGround* backGround = NewGO<BackGround>(0, "backGround");
				backGround->SetPosition(objData.position);
				backGround->SetRotation(objData.rotation);
				backGround->SetScale(objData.scale);

				return true;
			}

			// 名前がboxなら
			if (objData.EqualObjectName(L"box") == true) {
				// 壁を生成
				Wall_Normal* wall_normal = NewGO<Wall_Normal>(0, "wall_Normal");
				wall_normal->SetPosition(objData.position);
				wall_normal->SetRotation(objData.rotation);
				wall_normal->SetScale(objData.scale);
				m_wallList.emplace_back(wall_normal);
				return true;
			}

			// 名前がgapのとき
			if (objData.EqualObjectName(L"gap_1") == true) {
				// 隙間を生成する
				Wall_Gap* wall_gap = NewGO<Wall_Gap>(0, "wall_Gap");
				wall_gap->ModelLoad(1);
				wall_gap->SetPosition(objData.position);
				wall_gap->SetRotation(objData.rotation);
				wall_gap->SetScale(objData.scale);
				m_wallList.emplace_back(wall_gap);
				return true;
			}

			// 名前がgapのとき
			if (objData.EqualObjectName(L"gap_2") == true) {
				// 隙間を生成する
				Wall_Gap* wall_gap = NewGO<Wall_Gap>(0, "wall_Gap");
				wall_gap->ModelLoad(2);
				wall_gap->SetPosition(objData.position);
				wall_gap->SetRotation(objData.rotation);
				wall_gap->SetScale(objData.scale);
				m_wallList.emplace_back(wall_gap);
				return true;
			}

			// 名前がgapのとき
			if (objData.EqualObjectName(L"gap_3") == true) {
				// 隙間を生成する
				Wall_Gap* wall_gap = NewGO<Wall_Gap>(0, "wall_Gap");
				wall_gap->ModelLoad(3);
				wall_gap->SetPosition(objData.position);
				wall_gap->SetRotation(objData.rotation);
				wall_gap->SetScale(objData.scale);
				m_wallList.emplace_back(wall_gap);
				return true;
			}

			// 名前がpostのとき
			if (objData.EqualObjectName(L"post") == true) {
				// 隙間を生成する
				Wall_Post* post = NewGO<Wall_Post>(0, "wall_Post");
				post->SetPosition(objData.position);
				post->SetRotation(objData.rotation);
				post->SetScale(objData.scale);
				m_wallList.emplace_back(post);
				return true;
			}

			// 名前がdecorationのとき
			if (objData.EqualObjectName(L"decoration") == true) {
				// 障害物を生成
				Wall_Decoration* wall_Decoration = NewGO<Wall_Decoration>(0, "wall_Decoration");
				wall_Decoration->SetPosition(objData.position);
				wall_Decoration->SetRotation(objData.rotation);
				wall_Decoration->SetScale(objData.scale);
				m_wallList.emplace_back(wall_Decoration);
				return true;
			}

			// 名前がstartのとき
			if (objData.EqualObjectName(L"start") == true) {
				// スタートを生成
				Wall_Door* door = NewGO<Wall_Door>(0, "wall_Door");
				door->SetPosition(objData.position);
				door->SetRotation(objData.rotation);
				door->SetScale(objData.scale);
				m_wallList.emplace_back(door);
				return true;
			}

			// 名前がgoalのとき
			if (objData.EqualObjectName(L"clear") == true) {
				// ゴールを生成
				Wall_Normal* wall_normal = NewGO<Wall_Normal>(0, "wall_Normal");
				wall_normal->SetPosition(objData.position);
				wall_normal->SetRotation(objData.rotation);
				wall_normal->SetScale(objData.scale);
				m_wallList.emplace_back(wall_normal);
				m_position = objData.position;
				m_pointLight[lights].SetPointLight(
					lights,
					Vector3(m_position.x, m_position.y + 10.0f, m_position.z),
					Vector3::Zero,
					400.0f
				);
				lights++;
				return true;
			}
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
			Sensor* sensor = NewGO<Sensor>(0, "sensor");
			sensor->SetPosition(objData.position);
			sensor->SetScale(objData.scale);
			sensor->SetRotation(objData.rotation);
			m_sensorList.emplace_back(sensor);

			return true;
		}

		if (objData.EqualObjectName(L"camera") == true) {
			SecurityCamera* securityCamera = NewGO<SecurityCamera>(0, "securityCamera");
			securityCamera->SetPosition(objData.position);
			securityCamera->SetType(1);
			securityCamera->SetNumber(m_spotLigNum);
			m_spotLigNum++;
			m_SecurityCameraList.emplace_back(securityCamera);
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

			PhysicsGhost* physicsGhost = NewGO<PhysicsGhost>(0, "physicsGhost");
			physicsGhost->SetPosition(objData.position);
			physicsGhost->SetScale(objData.scale);
			physicsGhost->SetRotation(objData.rotation);
			m_physicsGhostList.emplace_back(physicsGhost);
			return true;
		}
		if (objData.EqualObjectName(L"physics") == true) {
			GhostBox* ghostBox = NewGO<GhostBox>(0,"ghostBox");
			ghostBox->SetPosition(objData.position);
			ghostBox->SetScale(objData.scale);
			ghostBox->SetRotation(objData.rotation);
		}
		return true;
		}
	);

	RenderingEngine::GetInstance()->GetLightCB().spNum = m_spotLigNum;
}

void Game::Update()
{		
	if (m_gameState == m_enGameState_GameClearable || m_gameState == m_enGameState_DuringGamePlay)
	{
		DuringGamePlayState();
	}
	if (m_gameState == m_enGameState_GameClearable)
	{
		ClearableState();
	}

	

	for (int i = 0; i < 3; i++) {
		m_pointLight[i].Update();
	}

	//フェードの待機時間
	if (m_isWaitFadeOut) {
		//フェードし終えたら
		if (!m_fade->IsFade()) {

			//BGMが削除されるまで待つ
			if (!GameManager::GetInstance()->IsDeleteBGM()) {
				return;
			}

			if (m_gameState == m_enGameState_EventStart) {
				//イベントシーンを呼ぶ
				Event* event = NewGO<Event>(0, "event");
				event->SetTresurePosition(m_tresurePos);

				GameManager::GetInstance()->SetGameState(GameManager::enState_Result);
				RenderingEngine::GetInstance()->GetLightCB().spNum = 0;
				NotDraw_Enemy(true);

				m_gameUI->Deactivate();
				m_gage->Deactivate();
				m_miniMap->Deactivate();
				m_pause->Deactivate();
				m_gamecamera->Deactivate();
				m_playerManagement->Deactivate();
				m_player3D->Deactivate();

				m_isWaitFadeOut = false;
				
				return;
			}

			if (m_gameState ==m_enGameState_GameBuck)
			{
				GamePos();
				
			}
			else if(m_gameState==m_enGameState_GameClear)
			{
				//ゲームクリアにする
				Result* result = NewGO<Result>(0, "result");
				result->SetResult(true);
				result->SetTime(m_gameUI->GetTime());
			}
			else if (m_gameState == m_enGameState_GameOver)
			{
				//ゲームオーバーにする
				Result* result = NewGO<Result>(0, "result");
				result->SetResult(false);
				result->SetTime(m_gameUI->GetTime());
			}
			DeleteGO(FindGO<Pause>("pause"));
			DeleteGO(this);
		}
	}
	

}
void Game::ClearableState()
{
	
	Vector3 diff = m_playerManagement->GetPosition() - m_pointLight[m_lightNumber].GetPosition();
	if (diff.LengthSq() <= 120.0f * 120.0f)
	{
		NotifyGameClear();
	
	}
}

void Game::NotDraw_Enemy(bool flag)
{
	// 描画するかどうか決定する
	for (int i = 0; i < m_enemyList.size(); i++) {
		m_enemyList[i]->SetNotDrawFlag(flag);
		m_enemyList[i]->SetTrueChangeDefaultFlag();
	}
}

void Game::DuringGamePlayState()
{
	if (m_gameUI->GetTime() <= 0.0f)
	{
		NotifyGameOver();
	}
}

void Game::NotifyGameStart()
{
	m_gameState = m_enGameState_GameStart;
}

void Game::NotifyDuringGamePlay()
{
	m_gameState = m_enGameState_DuringGamePlay;
}

void Game::NotifyEventStart()
{
	m_gameState = m_enGameState_EventStart;
	m_fade->StartFadeOut();
	GameManager::GetInstance()->DeleteBGM();
	m_isWaitFadeOut = true;
}

void Game::NotifyEventEnd()
{
	GameManager::GetInstance()->SetGameState(GameManager::enState_Game);
	RenderingEngine::GetInstance()->GetLightCB().spNum = m_spotLigNum;
	RenderingEngine::GetInstance()->GetLightCB().ptNum = 3;
	NotDraw_Enemy(false);

	m_gameUI->Activate();
	m_gage->Activate();
	m_miniMap->Activate();
	m_pause->Activate();
	m_gamecamera->Activate();
	m_playerManagement->Activate();
	m_player3D->Activate();

	m_fade->StartFadeIn();

	NotifyGameClearable();
}


void Game::NotifyGameClear()
{
	m_isWaitFadeOut = true;
	m_fade->StartFadeOut();
	m_gameState = m_enGameState_GameClear;
}

void Game::NotifyGameOver()
{
	m_isWaitFadeOut = true;
	m_fade->StartFadeOut();
	m_gameState = m_enGameState_GameOver;
}

void Game::NotifyGameClearable()
{
	m_gameState = m_enGameState_GameClearable;
}

void Game::NotifyGameBack()
{
	m_gameState = m_enGameState_GameBuck;
}