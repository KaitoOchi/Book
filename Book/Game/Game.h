#pragma once

class Player3D;
class Player2D;
class GameCamera;
class MiniMap;
class BackGround;
class Enemy;
class Enemy_Normal;
class Enemy_Search;
class Enemy_Charge;
class PlayerManagement;
class Wall;
class Treasure;
class Ghost;
class GameUI;
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void LevelDesign();			// ステージのレベルデザインの処理

	//クリア座標を返す
	Vector3 GetClearPosition()
	{
		return m_position;
	}
	void SetClearPosition(Vector3 m_pos)
	{
		m_position = m_pos;
	}
	
	Vector3 m_position = Vector3::Zero;
	enum EnGameState
	{
		m_enGameState_DuringGamePlay,//ゲームプレイ中
		m_enGameState_GameClearable,//ゲームクリア条件達成
		m_enGameState_GameClear,//ゲームクリア
		m_enGameState_GameOver,//ゲームオーバー
		m_enGameState_GameBuck//タイトルに戻る
	};
	EnGameState m_gameState = m_enGameState_DuringGamePlay;//現在のステートの管理
private:
	/// <summary>
	/// クリアステートに遷移
	/// </summary>
	void ClearState();
	/// <summary>
	/// クリア可能ステートに遷移
	/// </summary>
	void Clearable();

	/// <summary>
	/// ステートの切替
	/// </summary>
	void MnageState();
	
	ModelRender m_stageModelRender;
	PhysicsStaticObject m_demobg;
	Player3D* m_player3D = nullptr;//3Dプレイヤー
	Player2D* m_player2D = nullptr;//2Dプレイヤー
	GameCamera* m_gamecamera = nullptr;

	LevelRender m_levelRender;
	MiniMap* m_miniMap = nullptr;
	Enemy_Normal* m_enemyNormal = nullptr;
	Enemy_Search* m_enemySearch = nullptr;
	Enemy_Charge* m_enemyCharge = nullptr;
	BackGround* m_backGround = nullptr;

	std::vector<Enemy*> m_enemyList;
	


	PlayerManagement* m_playerManagement = nullptr;
	Wall* m_wall = nullptr;
	Treasure* m_treaSure = nullptr;
	Ghost* m_ghost = nullptr;
	GameUI* m_gameUI = nullptr;
	PointLight m_pointLight;

};

