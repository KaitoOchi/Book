#pragma once
#include "Wipe.h"

class Game;
class Enemy;
class Fade;
class Enemy_Increase;
class Player3D;

class Gage:public IGameObject
{
public:
	/// <summary>
	/// レベルステート。
	/// </summary>
	enum EnLeverState
	{
		m_enLever_1,
		m_enLever_2,
		m_enLever_3,
		m_enLever_MAX,
	};

public:
	Gage();
	~Gage();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ゲージの上昇処理。
	/// </summary>
	void GageUp(const int GageUp, const bool isEnemy);

	/// <summary>
	/// ワイプの終了処理。
	/// </summary>
	void SetWipeEnd()
	{
		m_wipe->WipeEnd();
		m_leverState = m_enLever_MAX;
	}

	/// <summary>
	/// レベルをMAXにする。
	/// </summary>
	void SetLeverMax()
	{
		m_leverState = m_enLever_MAX;
	}

private:
	
	/// <summary>
	/// ゲージの減少処理。
	/// </summary>
	void GageDown();

	/// <summary>
	/// 警戒度レベルの変更処理。
	/// </summary>
	void VigilaceLeverChange();
	
	/// <summary>
	/// ゲージの色を変更する。
	/// </summary>
	void Gage_ColorChange(); 

	/// <summary>
	/// ゲージをMAXにする。
	/// </summary>
	void Gage_MAX();

	/// <summary>
	/// ゲージの移動処理。
	/// </summary>
	void Gauge_Move();

private:
	SpriteRender					m_maxLeverRender;				//最大レベル画像。
	SpriteRender					m_baseRender;					//ベース画像。
	std::array< SpriteRender, 4 >	m_LeverUPRender;				//レベル画像。
	std::array< SpriteRender,10 >	m_vigilanceRender;				//警戒度ゲージ画像。
	Game*							m_game = nullptr;
	Wipe*							m_wipe = nullptr;
	Fade*							m_fade = nullptr;
	Player3D*						m_player3D = nullptr;
	Enemy_Increase*					m_enemy_Increase = nullptr;
	EnLeverState					m_leverState = m_enLever_1;
	EnLeverState					m_GetState = m_leverState;		//レベルステート。
	bool							m_isFind = false;				//見つかったかどうか。
	bool							m_MaxEnd = true;				// ?
	int								m_vigilanceGage = 0;			//警戒度ゲージ。
	float							m_vigilanceTime = 2.0f;			//警戒度の増加のクールダウンタイム。
	float							m_vigilaceDownTime = 2.0f;		//警戒度の減少のクールダウンタイム。
	float							m_HitTime = 5.0f;				//ヒットタイマー。
	float							m_gaugeTimer = 0.0f;			//ゲージタイマー。
	float							m_Color = 0.7f;					//ゲージ変更の色。
	float							m_gaugeScale = 0.0f;			//ゲージの拡大率。
};

