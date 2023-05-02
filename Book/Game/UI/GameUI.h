#pragma once

class PlayerManagement;
class Game;
class GameUI : public IGameObject	
{
public:
	GameUI();
	~GameUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 警戒値を上昇する。
	/// </summary>
	/// <param name="GageUp"></param>
	void Vigilance(int GageUp);

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
	/// 警戒度の変更処理。
	/// </summary>
	void VigilanceChange();
	void TimeMove();
private:
	SpriteRender		m_gageFrameSpriteRender;		//ゲージの枠画像
	SpriteRender		m_gageSpriteRender;				//ゲージ画像
	SpriteRender		m_vigilanceRender;				//警戒度画像
	FontRender			m_timeFontRender;				//タイム文字
	PlayerManagement*	m_playerManagement;
	Game*				m_game;
	int					m_vigilanceGage = 1;			//警戒度
	int					m_Gitgage = 0;
	float				m_timer = 0.0f;					//タイマー
	float				m_vigilanceTime = 0.0f;			//警戒度用のタイマー
	float				m_gage = 0.0f;					//MPゲージ
	float				m_gaugeTimer = 0.0f;			//ゲージ用のタイマー

	float				m_timePosition = 0.0f;			//タイムの座標

};

