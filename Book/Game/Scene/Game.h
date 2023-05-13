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
	EnGameSceneState m_gameState = m_enGameState_DuringGamePlay;	//ゲーム中

public:
	Game();
	~Game();
	bool Start();
	void Update();

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
	/// 描画するかどうか決定する。
	/// </summary>
	/// <param name="flag">trueなら描画しない。</param>
	void NotDraw_Enemy(bool flag);


	/// <summary>
	/// 宝の座標を設定。
	/// </summary>
	void SetTresurePosition(const Vector3& pos)
	{
		m_tresurePos = pos;
	}

	/// <summary>
	/// クリア座標の取得。
	/// </summary>
	Vector3& GetClearPosition()
	{
		return m_position;
	}

	/// <summary>
	/// ポイントライトを取得。
	/// </summary>
	/// <returns></returns>
	PointLight& GetPointLight()
	{
		return m_pointLight[m_lightNumber];
	}

	/// <summary>
	/// 敵リストの取得。
	/// </summary>
	std::vector<Enemy*> GetEnemyList()
	{
		return m_enemyList;
	}

	/// <summary>
	/// お宝座標リストの取得。
	/// </summary>
	/// <returns></returns>
	std::vector<Vector3> GetTreasurePositionList()
	{
		return m_treasurePositions;
	}

	/// <summary>
	/// 壁のリストを取得。
	/// </summary>
	std::vector<Wall*>GetWallList()
	{
		return m_wallList;
	}

	/// <summary>
	/// コリジョンのリストを取得。
	/// </summary>
	std::vector<PhysicsGhost*>GetPhysicsGhostList()
	{
		return m_physicsGhostList;
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
	/// クリア可能
	/// </summary>
	void ClearableState();
	/// <summary>
	/// ゲーム中
	/// </summary>
	void DuringGamePlayState();
	/// <summary>
	/// レベルレンダーの処理。
	/// </summary>
	void LevelDesign();

private:
	LevelRender						m_levelRender;					//レベルレンダー
	PointLight						m_pointLight[4];				//ポイントライト

	Player3D*						m_player3D = nullptr;			//3Dプレイヤー
	Player2D*						m_player2D = nullptr;			//2Dプレイヤー
	PlayerManagement*				m_playerManagement = nullptr;	//プレイヤーマネジメント
	GameCamera*						m_gamecamera = nullptr;			//カメラ
	Treasure*						m_treaSure = nullptr;			//お宝
	Fade*							m_fade = nullptr;				//フェード
	GameUI*							m_gameUI = nullptr;				//UI
	MiniMap*						m_miniMap = nullptr;			//ミニマップ
	Gage*							m_gage = nullptr;				//警戒度ゲージ
	Pause*							m_pause = nullptr;				//ポーズ画面

	std::vector<Enemy*>				m_enemyList;					//敵のリスト
	std::vector<Wall*>				m_wallList;						//壁のリスト
	std::vector<Vector3>			m_treasurePositions;			//お宝座標のリスト
	std::vector<Sensor*>			m_sensorList;					//ワイヤーのリスト
	std::vector<SecurityCamera*>	m_SecurityCameraList;			//監視カメラのリスト
	std::vector<PhysicsGhost*>		m_physicsGhostList;				//コリジョンのリスト

	Vector3							m_tresurePos;					//お宝の座標
	Vector3							m_position;						//クリアの座標
	bool							m_isWaitFadeOut = false;		//フェード状態かどうか
	int								m_nextScene = 0;				//次に移行するシーン
	int								m_spotLigNum = 0;				// エネミー用スポットライトの数
	int								lights = 0;
	int								m_lightNumber = 0;				//ポイントライトの数
};	