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
#include "Enemy_Increase.h"
#include "BackGround.h"
#include "Stage/Wall/Wall.h"
#include "Stage/Wall/Wall_Decoration.h"
#include "Stage/Wall/Wall_Door.h"
#include "Stage/Wall/Wall_Duct.h"
#include "Stage/Wall/Wall_Gap.h"
#include "Stage/Wall/Wall_Normal.h"
#include "Stage/Wall/Wall_Post.h"
#include "Treasure.h"
#include "Ghost.h"
#include "GhostBox.h"
#include "PhysicsGhost.h"
#include "Fade.h"
#include "Result.h"
#include <random>
#include"Gage.h"
#include "Pause.h"
#include "CountDown.h"
#include "SecurityCamera.h"
#include "Event.h"
#include "nature/SkyCube.h"
#include "GoalSprite.h"

Game::Game()
{
	//・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ阡ｻ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽL・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ・ｽE・ｽ
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	//煙のエフェクト
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/kemuri/kemuri.efk");
	//音と煙のエフェクト
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/otokemuri/otokemuri.efk");
	//星エフェクト
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/star/star.efk");
	//ビックリマーク
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/exclamation/exmark.efk");
	// はてなマークのエフェクト
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/effect/question/hatena.efk");
	//隙間のキラキラエフェクト
	EffectEngine::GetInstance()->ResistEffect(5, u"Assets/effect/sukima/sukima.efk");
	//お宝のキラキラエフェクト
	EffectEngine::GetInstance()->ResistEffect(6, u"Assets/effect/treasure/kirakira.efk");
	//疲れた時のエフェクト
	EffectEngine::GetInstance()->ResistEffect(7, u"Assets/effect/tire/tire.efk");
}

Game::~Game()
{
	for (int i = 0; i < m_physicsGhostList.size(); i++)
	{
		DeleteGO(m_physicsGhostList[i]);
	}

	//�E�G�E�l�E�~�E�[
	for (int i = 0; i < m_enemyList.size(); i++)
	{
		DeleteGO(m_enemyList[i]);
	}
	//�I�u�W�F�N�g
	//�E�I�E�u�E�W�E�F�E�N�E�g
	for (int i = 0; i < m_sensorList.size(); i++)
	{
		DeleteGO(m_sensorList[i]);
	}

	//UIの削除
	DeleteGO(m_gameUI);
	DeleteGO(m_gage);
	DeleteGO(m_miniMap);
	DeleteGO(m_gamecamera);

	DeleteGO(FindGO<GoalSprite>("goalSprite"));

	DeleteGO(FindGO<Enemy_Increase>("enemyIncrease"));

	//壁や床の削除
	DeleteGO(m_backGround);
	for (int i = 0; i < m_wallList.size(); i++)
	{
		DeleteGO(m_wallList[i]);
	}
	//監視カメラの削除
	for (int i = 0; i < m_SecurityCameraList.size(); i++)
	{
		DeleteGO(m_SecurityCameraList[i]);
	}
	//お宝の削除
	DeleteGO(m_treasure);

	//プレイヤーの削除
	DeleteGO(m_player3D);
	DeleteGO(m_player2D);
	DeleteGO(m_playerManagement);

	//リストの初期化
	m_enemyList.clear();
	m_wallList.clear();
	m_physicsGhostList.clear();
	m_sensorList.clear();
	m_SecurityCameraList.clear();

	// ライトの数を0に
	RenderingEngine::GetInstance()->GetLightCB().ptNum = 0;
	RenderingEngine::GetInstance()->GetLightCB().spNum = 0;
}

bool Game::Start()
{
	//スタート時を知らせる
	NotifyGameStart();

	m_player2D = NewGO<Player2D>(0,"player2d");
	m_player3D = NewGO<Player3D>(0, "player3d");
	m_gamecamera=NewGO<GameCamera>(0, "gameCamera");
	m_playerManagement = NewGO<PlayerManagement>(0, "playerManagement");
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	m_gage = NewGO<Gage>(0,"gage");
	NewGO<CountDown>(0, "countDown");
	
	NewGO<SkyCube>(0, "skyCube");

	RenderingEngine::GetInstance()->GetLightCB().ptNum = 3;

	NewGO<Enemy_Increase>(0, "enemyIncrease");

	//お宝を作成する
	m_treasure = NewGO<Treasure>(0, "treaSure");
	m_miniMap = NewGO<MiniMap>(0, "miniMap");

	LevelDesign();

	//お宝の座標をランダムで決める
	m_treasurePos = m_treasure->SetTreasurePosition();
	//決めた座標をミニマップに反映
	m_miniMap->SetTreasurePos(m_treasurePos);

	//ポイントライトの設定
	m_pointLight.SetPointLight(
		0,
		Vector3(m_treasurePos.x, m_treasurePos.y + 10.0f, m_treasurePos.z),
		Vector3(1.0f, 1.0f, 0.0f),
		400.0f
	);

	//フェードイン処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void Game::LevelDesign()
{
	//クリア地点を0～2でランダムに生成
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int>dist(0, 2);
	int clearNumber = dist(mt);
	int clearNumTmp = 0;

	// レベルデザイン処理
	m_levelRender.Init("Assets/level3D/level0_1.tkl", [&](LevelObjectData& objData){
		//名前がプレイヤーの時
		if (objData.EqualObjectName(L"Player") == true)
		{
			m_player3D->SetPosition(objData.position);
			m_player3D->SetRotation(objData.rotation);
			return true;
		}

		// 名前が Normal のとき
		if (objData.ForwardMatchName(L"Normal") == true) {
			 //エネミーを生成
			Enemy_Normal* enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			// 自身の属性を教える
			enemyNormal->m_enemyType = Enemy::TYPE_NORMAL;
			// 座標・回転・スケールを教える
			enemyNormal->SetPosition(objData.position);
			enemyNormal->SetRotation(objData.rotation);
			enemyNormal->SetScale(objData.scale);

			enemyNormal->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			enemyNormal->Pass(objData.number);
			// 初期座標を記憶する
			m_enemyFirstPositions.push_back(objData.position);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemyNormal);
			return true;
		}

		// 名前が Charge のとき
		if (objData.ForwardMatchName(L"Charge") == true) {
			// エネミーを生成
			Enemy_Charge* enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
			// 自身の属性を教える
			enemyCharge->m_enemyType = Enemy::TYPE_CHARGE;
			// 座標・回転・スケールを教える
			enemyCharge->SetPosition(objData.position);
			enemyCharge->SetRotation(objData.rotation);
			enemyCharge->SetScale(objData.scale);

			enemyCharge->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			enemyCharge->Pass(objData.number);
			// 初期座標を記憶する
			m_enemyFirstPositions.push_back(objData.position);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemyCharge);
			return true;
		}

		// 名前が Search のとき
		if (objData.EqualObjectName(L"Search") == true) {
			// エネミーを生成
			Enemy_Search* enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
			// 自身の属性を教える
			enemySearch->m_enemyType = Enemy::TYPE_SEARCH;
			// 座標・回転・スケールを教える
			enemySearch->SetPosition(objData.position);
			enemySearch->SetRotation(objData.rotation);
			enemySearch->SetScale(objData.scale);

			enemySearch->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// 初期座標を記憶する
			m_enemyFirstPositions.push_back(objData.position);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemySearch);
			return true;
		}
		
		// 名前が Clear のとき
		if (objData.ForwardMatchName(L"Clear") == true) {
			// エネミーを生成
			Enemy_Clear* enemyClear = NewGO<Enemy_Clear>(0, "enemyClear");
			// 自身の属性を教える
			enemyClear->m_enemyType = Enemy::TYPE_CLEAR;
			// 座標・回転・スケールを教える
			enemyClear->SetPosition(objData.position);
			enemyClear->SetRotation(objData.rotation);
			enemyClear->SetScale(objData.scale);

			enemyClear->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			enemyClear->Pass(objData.number);
			// 初期座標を記憶する
			m_enemyFirstPositions.push_back(objData.position);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemyClear);
			return true;
		}

		//名前がAddNormalのとき
		if (objData.ForwardMatchName(L"AddNormal") == true) {
			//エネミーを生成
			Enemy_Normal* enemyNormal = NewGO<Enemy_Normal>(0, "enemyNormal");
			// 自身の属性を教える
			enemyNormal->m_enemyType = Enemy::TYPE_NORMAL;
			// 座標・回転・スケールを教える
			enemyNormal->SetPosition(objData.position);
			enemyNormal->SetRotation(objData.rotation);
			enemyNormal->SetScale(objData.scale);

			enemyNormal->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			enemyNormal->Pass(objData.number);
			// 初期座標を記憶する
			m_enemyFirstPositions.push_back(objData.position);
			//追加する前なので描画しない
			enemyNormal->SetActiveFlag(true);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemyNormal);
			return true;
		}

		if (objData.ForwardMatchName(L"AddCharge") == true) {
			// エネミーを生成
			Enemy_Charge* enemyCharge = NewGO<Enemy_Charge>(0, "enemyCharge");
			// 自身の属性を教える
			enemyCharge->m_enemyType = Enemy::TYPE_CHARGE;
			// 座標・回転・スケールを教える
			enemyCharge->SetPosition(objData.position);
			enemyCharge->SetRotation(objData.rotation);
			enemyCharge->SetScale(objData.scale);
			enemyCharge->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			enemyCharge->Pass(objData.number);
			// 初期座標を記憶する
			m_enemyFirstPositions.push_back(objData.position);
			//追加前なので描画しない
			enemyCharge->SetActiveFlag(true);

			// エネミーのリストに追加する
			m_enemyList.push_back(enemyCharge);
			return true;
		}

		if (objData.ForwardMatchName(L"AddClear") == true) {
			// エネミーを生成
			Enemy_Clear* enemyClear = NewGO<Enemy_Clear>(0, "enemyClear");
			// 自身の属性を教える
			enemyClear->m_enemyType = Enemy::TYPE_CLEAR;
			// 座標・回転・スケールを教える
			enemyClear->SetPosition(objData.position);
			enemyClear->SetRotation(objData.rotation);
			enemyClear->SetScale(objData.scale);

			enemyClear->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			// パス移動の順路を指定
			enemyClear->Pass(objData.number);
			enemyClear->SetActiveFlag(true);
			// 初期座標を記憶する
			m_enemyFirstPositions.push_back(objData.position);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemyClear);
			return true;
		}
		// 名前が Search のとき
		if (objData.ForwardMatchName(L"AddSearch") == true) {
			// エネミーを生成
			Enemy_Search* enemySearch = NewGO<Enemy_Search>(0, "enemySearch");
			// 自身の属性を教える
			enemySearch->m_enemyType = Enemy::TYPE_SEARCH;
			// 座標・回転・スケールを教える
			enemySearch->SetPosition(objData.position);
			enemySearch->SetRotation(objData.rotation);
			enemySearch->SetScale(objData.scale);

			enemySearch->SetSpotLigNum(m_spotLigNum);
			m_spotLigNum++;
			enemySearch->SetActiveFlag(true);
			// 初期座標を記憶する
			m_enemyFirstPositions.push_back(objData.position);
			// エネミーのリストに追加する
			m_enemyList.push_back(enemySearch);
			return true;
		}
			// ステージのレベル
		{
			//名前がbackgroundなら
			if (objData.EqualObjectName(L"base") == true){
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
				Wall_Normal* normal = NewGO<Wall_Normal>(0, "wall_Normal");
				normal->SetPosition(objData.position);
				normal->SetRotation(objData.rotation);
				normal->SetScale(objData.scale);
				m_wallList.emplace_back(normal);
				return true;
			}

			// 名前がgapのとき
			if (objData.EqualObjectName(L"gap") == true) {
				// 隙間を生成する
				Wall_Gap* gap = NewGO<Wall_Gap>(0, "wall_Gap");
				gap->SetPosition(objData.position);
				gap->SetRotation(objData.rotation);
				gap->SetScale(objData.scale);
				m_wallList.emplace_back(gap);
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
			if (objData.ForwardMatchName(L"decoration") == true) {
				// 障害物を生成
				Wall_Decoration* decoration = NewGO<Wall_Decoration>(0, "wall_Decoration");
				// モデルの番号を渡す
				decoration->Load_Model(objData.number);
				decoration->SetPosition(objData.position);
				decoration->SetRotation(objData.rotation);
				decoration->SetScale(objData.scale);
				m_wallList.emplace_back(decoration);
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
				Wall_Duct* duct = NewGO<Wall_Duct>(0, "wall_Duct");
				duct->SetPosition(objData.position);
				duct->SetRotation(objData.rotation);
				duct->SetScale(objData.scale);
				m_wallList.emplace_back(duct);

				if (clearNumber == clearNumTmp) {
					m_clearPos = objData.position;
				}
				clearNumTmp++;
				return true;
			}
		}
		if (objData.EqualObjectName(L"sensor")==true) {
			Sensor* sensor = NewGO<Sensor>(0, "sensor");
			sensor->SetPosition(objData.position);
			sensor->SetScale(objData.scale);
			sensor->SetRotation(objData.rotation);
			m_sensorList.emplace_back(sensor);

			return true;
		}

		if (objData.ForwardMatchName(L"camera") == true) {
			SecurityCamera* securityCamera = NewGO<SecurityCamera>(0, "securityCamera");
			securityCamera->SetPosition(objData.position);
			securityCamera->SetType(objData.number);
			securityCamera->SetNumber(m_spotLigNum);
			m_spotLigNum++;
			m_SecurityCameraList.emplace_back(securityCamera);
			return true;
		}
		if (objData.EqualObjectName(L"push") == true) {
			//プレイヤーを押し出す先の座標を与える
			m_player3D->m_ghostpositions.push_back(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"otakara") == true) {

			m_treasure->SetTreasureList(objData.position);
			
			return true;
		}
		if (objData.EqualObjectName(L"ghost") == true) {

			m_physicsGhost = NewGO<PhysicsGhost>(0, "physicsGhost");
			m_physicsGhost->SetPosition(objData.position);
			m_physicsGhost->SetScale(objData.scale);
			m_physicsGhost->SetRotation(objData.rotation);
			m_physicsGhostList.emplace_back(m_physicsGhost);
			
			return true;
		}
		if (objData.EqualObjectName(L"physics") == true) {
			//透明なブロックを作成する
			m_ghostBox = NewGO<GhostBox>(0,"ghostBox");
			m_ghostBox->SetPosition(objData.position);
			m_ghostBox->SetScale(objData.scale);
			m_ghostBox->SetRotation(objData.rotation);
		}
		return true;
		}
	);
	RenderingEngine::GetInstance()->GetLightCB().spNum = m_spotLigNum;

	m_grassModelRender.Init("Assets/modelData/level_test/grass.tkm");
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

	//フェードの待機時間
	if (m_isWaitFadeOut) {
		//フェードし終えたら
		if (!m_fade->IsFade()) {

			//BGMが削除されるまで待つ
			if (!GameManager::GetInstance()->IsDeleteBGM()) {
				return;
			}

			ExitGame();
		}
	}
	

}
void Game::ClearableState()
{
	
	Vector3 diff = m_playerManagement->GetPosition() - m_pointLight.GetPosition();
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

		for (int j = 0; j < m_enemyFirstPositions.size(); j++) {
			// 座標を教える
			m_enemyList[i]->SetPosition(m_enemyFirstPositions[i]);
		}
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

	//ステートをゲーム状態にする
	GameManager::GetInstance()->SetGameState(GameManager::enState_Game);

	//BGMを設定
	GameManager::GetInstance()->SetBGM(21);
}

void Game::NotifyDuringGamePlay()
{
	m_gameState = m_enGameState_DuringGamePlay;
}

void Game::NotifyEventStart()
{
	//クリア可能にする
	NotifyGameClearable();

	m_gameState = m_enGameState_EventStart;
	m_isWaitFadeOut = true;

	//敵を非表示
	NotDraw_Enemy(true);

	//ヒント画像を表示しない
	m_fade->SetEnableTips(false);

	//フェードアウトを開始
	m_fade->StartFadeOut();

	//BGMの削除
	GameManager::GetInstance()->DeleteBGM();

	//隙間エフェクトを非表示
	StopWallEffect();
}

void Game::NotifyEventEnd()
{
	//ステートをお宝取得後に設定
	GameManager::GetInstance()->SetGameState(GameManager::enState_GetTreasure);

	//ライトを戻す
	RenderingEngine::GetInstance()->GetLightCB().spNum = m_spotLigNum;
	RenderingEngine::GetInstance()->GetLightCB().ptNum = 3;

	//ライトを変更
	RenderingEngine::GetInstance()->SetDirectionLight(
		Vector3(1, -1, 1),
		Vector3(0.1f, 0.1f, 0.1f)
	);
	RenderingEngine::GetInstance()->SetHemiSphereLight(
		Vector3(0.2f, 0.1f, 0.1f),
		Vector3(0.3f, 0.1f, 0.1f),
		Vector3(0.0f, 1.0f, 0.0f)
	);

	//敵を表示する
	NotDraw_Enemy(false);

	//クラスをアクティブ状態にする
	m_gamecamera->Activate();
	m_gameUI->Activate();
	m_gage->Activate();
	m_miniMap->Activate();
	m_playerManagement->Activate();
	m_player3D->Activate();
	m_player3D->m_Player_Act = true;
	m_player2D->m_Player_Act = true;

	//ミニマップに脱出口を表示
	m_miniMap->SetTreasurePos(m_clearPos);

	//警戒度レベルをMAXにする
	m_gage->m_leverState = m_gage->m_enLever_MAX;

	//隙間エフェクトを表示
	PlayWallEffect();

	//フェードインを開始
	m_fade->StartFadeIn();
	NotifyGameClearable();
}


void Game::NotifyGameClear()
{
	if (!m_isWaitFadeOut) {
		//フェードアウトを開始する
		m_isWaitFadeOut = true;
		m_fade->StartFadeOut();

		//BGMの削除
		GameManager::GetInstance()->DeleteBGM();

		//ステートをクリアステートにする
		m_gameState = m_enGameState_GameClear;
	}
}

void Game::NotifyGameOver()
{
	if (!m_isWaitFadeOut) {
		//フェードアウトを開始する
		m_isWaitFadeOut = true;
		m_fade->StartFadeOut();

		//ヒント画像を表示する
		m_fade->SetEnableTips(true);

		//BGMの削除
		GameManager::GetInstance()->DeleteBGM();

		//ステートをクリアステートにする
		m_gameState = m_enGameState_GameOver;
	}
}

void Game::NotifyGameClearable()
{
	m_gameState = m_enGameState_GameClearable;
}

void Game::NotifyGameBack()
{
	m_gameState = m_enGameState_GameBuck;
}

void Game::ExitGame()
{
	//イベントの開始処理が呼ばれているなら
	if (m_gameState == m_enGameState_EventStart) {
		//イベントシーンを呼ぶ
		Event* event = NewGO<Event>(0, "event");
		event->SetTresurePosition(m_treasurePos);

		//ライトを非表示
		GameManager::GetInstance()->SetGameState(GameManager::enState_Result);
		RenderingEngine::GetInstance()->GetLightCB().spNum = 0;

		//脱出口にポイントライトを置く
		m_pointLight.SetPosition(Vector3(m_clearPos.x, m_clearPos.y + 10.0f, m_clearPos.z));
		m_pointLight.Update();

		m_gameUI->Deactivate();

		m_gage->Deactivate();
		m_gamecamera->Deactivate();
		m_miniMap->Deactivate();
		m_player3D->Deactivate();
		m_playerManagement->Deactivate();

		m_isWaitFadeOut = false;

		return;
	}

	//ポーズ画面が呼ばれているなら
	if (m_gameState == m_enGameState_GameBuck)
	{
		GamePos();
	}
	//ゲームクリアなら
	else if (m_gameState == m_enGameState_GameClear)
	{
		//ゲームクリアにする
		Result* result = NewGO<Result>(0, "result");
		result->SetResult(true);
		result->SetTime(m_gameUI->GetTime());
	}
	//ゲームオーバーが呼ばれているなら
	else if (m_gameState == m_enGameState_GameOver)
	{
		//ゲームオーバーにする
		Result* result = NewGO<Result>(0, "result");
		result->SetResult(false);
		result->SetTime(m_gameUI->GetTime());
	}

	//ライトを戻す
	RenderingEngine::GetInstance()->SetDirectionLight(Vector3(1, -1, 1), Vector3(0.2f, 0.2f, 0.2f));

	RenderingEngine::GetInstance()->SetHemiSphereLight(
		Vector3(0.3f, 0.3f, 0.4f),
		Vector3(0.1f, 0.1f, 0.2f),
		Vector3(0.0f, 1.0f, 0.0f)
	);

	DeleteGO(FindGO<Pause>("pause"));
	DeleteGO(this);
}

void Game::Render(RenderContext& rc)
{
	m_grassModelRender.Draw(rc);
}