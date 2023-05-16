#pragma once

class PlayerManagement;
class Game;
class Player3D;
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

	/// <summary>
	/// 閃光弾の数
	/// </summary>
	/// <param name="flash">現在の閃光弾の数</param>
	/// <returns></returns>
	const void SetFlashSlot(const int flash)
	{
		m_flashNumber = flash;
	}

	/// <summary>
	/// 音爆弾の数
	/// </summary>
	/// <param name="flash">現在の音爆弾の数</param>
	/// <returns></returns>
	const void SetSoundSlot(const int sound)
	{
		m_soundNumber = sound;
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

	/// <summary>
	/// アイテムの所持数の処理
	/// </summary>
	void ItemSlot();

	void ItemScaleUp();

private:
	SpriteRender		m_gageFrameSpriteRender;		//ゲージの枠画像
	SpriteRender		m_gageSpriteRender;				//ゲージ画像
	FontRender			m_timeFontRender;				//タイム文字
	
	SpriteRender		m_itemBaseRender;				//アイテムの背景画像
	SpriteRender		m_itemFlashRender;				//閃光弾の画像
	SpriteRender		m_itemSoundRender;				//音爆弾の画像
	FontRender			m_itemFlashNumber;				//閃光弾の数
	FontRender			m_itemSoundNumber;				//音爆弾の数


	PlayerManagement*	m_playerManagement;
	Game*				m_game;
	Player3D*			m_player3D;

	bool				m_isTimerEnable = true;			//タイマーの表示状態
	int					m_vigilanceGage = 1;			//警戒度
	float				m_timer = 0.0f;					//タイマー
	float				m_vigilanceTime = 0.0f;			//警戒度用のタイマー
	float				m_gage = 0.0f;					//MPゲージ
	float				m_gaugeTimer = 0.0f;			//ゲージ用のタイマー
	float				m_timePosition = 0.0f;			//タイムの座標

	int					m_flashNumber = 0;				//閃光弾の数
	int					m_soundNumber = 0;				//音爆弾の数

	float				m_flashScale = 1.0f;			//閃光弾の画像の大きさ
	float				m_soundScale = 1.0f;			//閃光弾の画像の大きさ

	bool				m_falshState = true;			//閃光弾のステート
	bool				m_soundState = false;			//音爆弾のステート
	
};

