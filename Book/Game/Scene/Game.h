#pragma once

class Player3D;
class Player2D;
class GameCamera;
class MiniMap;
class BackGround;
class Enemy;
class PlayerManagement;
class Wall;
class Treasure;
class Ghost;
class GameUI;
class Gage;
class Pause;
class GhostBox;
class PhysicsGhost;
class Sensor;
class SecurityCamera;
#include "PhysicsGhost.h"
#include "GameManager.h"
#include "Title.h"
#include "Fade.h"

class Game : public IGameObject
{
public:
	enum EnGameSceneState
	{
		m_enGameState_GameStart,		//ゲームスタート時
		m_enGameState_DuringGamePlay,	//ゲーム中
		m_enGameState_EventStart,		//イベント開始
		m_enGameState_EventEnd,			//イベント終了
		m_enGameState_GameClearable,	//クリア可能
		m_enGameState_GameClear,		//クリア
		m_enGameState_GameFade,			//ゲームフェード
		m_enGameState_GameOver,			//ゲームオーバー
		m_enGameState_GameBuck			//他のメニューに戻る
	};
	EnGameSceneState m_gameState = m_enGameState_DuringGamePlay;//ゲーム中

public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// 描画するかどうか決定する。trueのとき描画しない
	/// </summary>
	/// <param name="flag"></param>
	void NotDraw_Enemy(bool flag);

	/// <summary>
	/// 宝の座標を設定。
	/// </summary>
	void SetTresurePosition(const Vector3& pos)
	{
		m_treasurePos = pos;
	}

	/// <summary>
	/// 敵リストの取得。
	/// </summary>
	/// <returns></returns>
	inline std::vector<Enemy*> GetEnemyList()
	{
		return m_enemyList;
	}

	/// <summary>
	/// 壁リストの取得。
	/// </summary>
	std::vector<Wall*>GetWallList()
	{
		return m_wallList;
	}

	/// <summary>
	/// 当たり判定リストの取得。
	/// </summary>
	/// <returns></returns>
	std::vector<PhysicsGhost*>GetPhysicsGhostList()
	{
		return m_physicsGhostList;
	}

	/// <summary>
	/// 隙間エフェクトを再生。
	/// </summary>
	void PlayWallEffect()
	{
		for (auto& effect : m_physicsGhostList)
		{
			effect->GetEffect()->Play();
		}
	}

	/// <summary>
	/// 隙間エフェクトを停止。
	/// </summary>
	void StopWallEffect()
	{
		for (auto& effect : m_physicsGhostList)
		{
			effect->GetEffect()->Stop();
		}
	}

public:
	/// <summary>
	/// ゲームの終了処理。
	/// </summary>
	void GameDelete(const int nextScene)
	{
		m_nextScene = nextScene;
		m_isWaitFadeOut = true;
		m_fade->SetEnableTips(true);
		m_fade->StartFadeOut();
		GameManager::GetInstance()->DeleteBGM();
	}

	/// <summary>
	/// ポーズ画面の切替
	/// </summary>
	void GamePos()
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


public:
	/// <summary>
	/// ゲームスタート
	/// </summary>
	void NotifyGameStart();
	/// <summary>
	/// ゲーム中
	/// </summary>
	void NotifyDuringGamePlay();
	/// <summary>
	/// イベントの開始を通知。
	/// </summary>
	void NotifyEventStart();
	/// <summary>
	/// イベントの終了を通知。
	/// </summary>
	void NotifyEventEnd();
	/// <summary>
	/// ゲームオーバーを通知
	/// </summary>
	void NotifyGameOver();
	/// <summary>
	/// ゲームクリアを通知
	/// </summary>
	void NotifyGameClear();
	/// <summary>
	/// ゲームバックを通知
	/// </summary>
	void NotifyGameBack();
	/// <summary>
	/// クリア可能を通知
	/// </summary>
	void NotifyGameClearable();
private:
	/// <summary>
	/// レベルレンダーの処理。
	/// </summary>
	void LevelDesign();
	/// <summary>
	/// クリア可能
	/// </summary>
	void ClearableState();
	/// <summary>
	/// ゲーム中
	/// </summary>
	void DuringGamePlayState();
	/// <summary>
	/// ゲームの終了処理。
	/// </summary>
	void ExitGame();

private:
	ModelRender						m_grassModelRender;				//草
	LevelRender						m_levelRender;					//レベルレンダー
	PointLight						m_pointLight;					//ポイントライト

	Player2D*						m_player2D = nullptr;
	Player3D*						m_player3D = nullptr;
	PlayerManagement*				m_playerManagement = nullptr;
	BackGround*						m_backGround = nullptr;
	Treasure*						m_treasure = nullptr;
	Ghost*							m_ghost = nullptr;
	PhysicsGhost*					m_physicsGhost = nullptr;
	GhostBox*						m_ghostBox = nullptr;
	GameCamera*						m_gamecamera = nullptr;
	Fade*							m_fade = nullptr;
	GameUI*							m_gameUI = nullptr;
	Gage*							m_gage = nullptr;
	MiniMap*						m_miniMap = nullptr;


	std::vector<Enemy*>				m_enemyList;
	std::vector<Vector3>			m_enemyFirstPositions;			// エネミーの初期座標
	std::vector<Wall*>				m_wallList;
	std::vector<Sensor*>			m_sensorList;
	std::vector<SecurityCamera*>	m_SecurityCameraList;
	std::vector<PhysicsGhost*>		m_physicsGhostList;

	Vector3							m_treasurePos;					//宝座標
	Vector3							m_clearPos;						//クリア座標

	bool							m_isWaitFadeOut = false;		//フェード状態かどうか
	int								m_nextScene = 0;				//次に移行するシーン
	int								m_spotLigNum = 0;				// エネミー用スポットライトの数
};	

