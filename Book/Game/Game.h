#pragma once
class Player3D;
class GameCamera;
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
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
	Player3D* m_player3D = nullptr;
	GameCamera* m_gamecamera = nullptr;

};

