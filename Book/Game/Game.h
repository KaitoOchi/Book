#pragma once
class Player3D;
class Player2D;
class GameCamera;
class Enemy;
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void LevelDesign();			// ステージのレベルデザインの処理
	enum EnGameState
	{
		m_enGameState_DuringGamePlay,//ゲームプレイ中
		m_enGameState_GameClear,//ゲームクリア
		m_enGameState_GameOver,//ゲームオーバー
		m_enGameState_GameBuck//タイトルに戻る
	};
private:
	/// <summary>
	/// ステートの切替
	/// </summary>
	void MnageState();
	EnGameState m_gameState=m_enGameState_DuringGamePlay;//現在のステートの管理
	ModelRender m_stageModelRender;
	PhysicsStaticObject m_demobg;
	Player3D* m_player3D = nullptr;//3Dプレイヤー
	Player2D* m_player2D = nullptr;//2Dプレイヤー
	GameCamera* m_gamecamera = nullptr;

	LevelRender* m_levelRender = nullptr;
};

