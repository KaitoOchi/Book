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
class Fade;
class GameUI;
class Gage;
class Pause;
class GhostBox;
class PhysicsGhost;
class Sensor;
class SecurityCamera;

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

public:
	/// <summary>
	/// 描画するかどうか決定する。trueのとき描画しない
	/// </summary>
	/// <param name="flag"></param>
	void NotDraw_Enemy(bool flag);

	/// <summary>
	/// クリア座標の設定。
	/// </summary>
	void SetClearPosition(Vector3 m_pos)
	{
		m_position = m_pos;
	}

	/// <summary>
	/// クリア座標の取得。
	/// </summary>
	const Vector3& GetClearPosition()
	{
		return m_position;
	}

	/// <summary>
	/// 宝の座標を設定。
	/// </summary>
	void SetTresurePosition(const Vector3& pos)
	{
		m_tresurePos = pos;
	}

	/// <summary>
	/// ポイントライトの取得。
	/// </summary>
	/// <returns></returns>
	PointLight& GetPointLight()
	{
		return m_pointLight[m_lightNumber];
	}

	/// <summary>
	/// 敵リストの取得。
	/// </summary>
	/// <returns></returns>
	std::vector<Enemy*> GetEnemyList()
	{
		return m_enemyList;
	}

	/// <summary>
	/// 宝座標リストの取得。
	/// </summary>
	std::vector<Vector3> GetTreasurePositionList()
	{
		return m_treasurePositions;
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

public:
	/// <summary>
	/// フェードアウト
	/// </summary>
	void GameDelete(const int nextScene);

	/// <summary>
	/// ポーズ画面の切替
	/// </summary>
	void GamePos();


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

private:

	LevelRender						m_levelRender;					//レベルレンダー
	std::array<PointLight, 4>		m_pointLight;					//ポイントライト

	Player3D*						m_player3D = nullptr;
	Player2D*						m_player2D = nullptr;
	PlayerManagement*				m_playerManagement = nullptr;
	BackGround*						m_backGround = nullptr;
	Treasure*						m_treaSure = nullptr;
	Ghost*							m_ghost = nullptr;
	PhysicsGhost*					m_physicsGhost = nullptr;
	GhostBox*						m_ghostBox = nullptr;
	GameCamera*						m_gamecamera = nullptr;
	Fade*							m_fade = nullptr;
	GameUI*							m_gameUI = nullptr;
	Gage*							m_gage = nullptr;
	MiniMap*						m_miniMap = nullptr;

	std::vector<Enemy*>				m_enemyList;
	std::vector<Wall*>				m_wallList;
	std::vector<Vector3>			m_treasurePositions;
	std::vector<Sensor*>			m_sensorList;
	std::vector<SecurityCamera*>	m_SecurityCameraList;
	std::vector<PhysicsGhost*>		m_physicsGhostList;

	Vector3							m_tresurePos;					//宝座標
	Vector3							m_position;						//クリア座標

	bool							m_isWaitFadeOut = false;		//フェード状態かどうか
	int								m_nextScene = 0;				//次に移行するシーン
	int								m_spotLigNum = 0;				// エネミー用スポットライトの数
	int								lights = 0;						//ポイントライトの数
	int								m_lightNumber = 0;				//現在のポイントライトの数
};	

