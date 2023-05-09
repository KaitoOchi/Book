#pragma once

class PlayerManagement;
class Game;
class Player2D;
class GameUI : public IGameObject	
{
public:
	GameUI();
	~GameUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// タイムを取得。
	/// </summary>
	const float GetTime()
	{
		return m_timer;
	}

private:
	/// <summary>
	/// 時間計測処理。
	/// </summary>
	void Time();

	/// <summary>
	/// ゲージの変更処理。
	/// </summary>
	void ChangeGage();

	/// <summary>
	/// 時間の移動処理。
	/// </summary>
	void TimeMove(const int& m, const int& s);

private:
	SpriteRender		m_gageFrameSpriteRender;		//ゲージの枠画像
	SpriteRender		m_gageSpriteRender;				//ゲージ画像
	FontRender			m_timeFontRender;				//タイム文字
	PlayerManagement*	m_playerManagement;
	Game*				m_game;
	Player2D*			m_player2D;
	bool				m_isTimerEnable = true;			//タイマーの表示状態
	int					m_vigilanceGage = 1;			//警戒度
	float				m_timer = 0.0f;					//タイマー
	float				m_vigilanceTime = 0.0f;			//警戒度用のタイマー
	float				m_gage = 0.0f;					//MPゲージ
	float				m_gaugeTimer = 0.0f;			//ゲージ用のタイマー
	float				m_timePosition = 0.0f;			//タイムの座標

};

