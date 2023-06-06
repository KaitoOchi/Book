#pragma once
#include "GameManager.h"

class Game;

/// <summary>
/// ポーズクラス。
/// </summary>
class Pause : public IGameObject
{
public:
	enum PauseState
	{
		enState_FadeIn,
		enState_FadeOut,
		enState_Pause,
		enState_Game
	};

public:
	Pause();
	~Pause();
	bool Start();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// ポーズ中のアップデート処理。
	/// </summary>
	void PauseUpdate();

	/// <summary>
	/// ポーズ中の処理。
	/// </summary>
	void PauseScreen();

	/// <summary>
	/// カーソルの移動処理。
	/// </summary>
	void CursorMove();

	/// <summary>
	/// フェードイン処理。
	/// </summary>
	void FadeIn();

	/// <summary>
	/// フェードアウト処理。
	/// </summary>
	void FadeOut();

	/// <summary>
	/// スプライトの変更処理。
	/// </summary>
	void SetSprite();

	/// <summary>
	/// SEを出す。
	/// </summary>
	void PlaySE(const int num)
	{
		SoundSource* se = NewGO<SoundSource>(0, "sound");
		se->Init(num);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
	}

private:
	SpriteRender	m_backGroundSpriteRender;		//背景画像
	SpriteRender	m_pauseSpriteRender;			//ポーズメニュー画像
	SpriteRender	m_cursorSpriteRender;			//カーソル画像
	PauseState		m_pauseState = enState_Game;	//ポーズステート
	Game*			m_game = nullptr;				//ゲームクラス
	Vector3			m_cursorPos;					//カーソル座標
	bool			m_isPause = false;				//ポーズしたかどうか
	int				m_cursor = 0;					//カーソル
	int				m_nextCursor = 0;				//次のカーソル
	float			m_timer = 0.0f;					//タイマー
	float			m_cursorTimer = 1.0f;			//カーソルタイマー
};