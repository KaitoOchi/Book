#pragma once

class PlayerManagement;
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
	void Time();
	void ChangeGage();
	void VigilanceChange();

private:
	SpriteRender		m_gageFrameSpriteRender;
	SpriteRender		m_gageSpriteRender;
	SpriteRender		m_vigilanceRender;
	FontRender			m_timeFontRender;
	PlayerManagement*	m_playerManagement;
	int					m_vigilanceGage = 1;			//警戒度
	int					m_Gitgage = 0;
	float				m_timer = 0.0f;					//タイマー
	float				m_vigilanceTime = 0.0f;			//警戒度用のタイマー
	float				m_gage = 0.0f;					//MPゲージ
	float				m_gaugeTimer = 0.0f;			//ゲージ用のタイマー
};

