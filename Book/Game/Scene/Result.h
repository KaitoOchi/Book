#pragma once
#include "level2DRender/Level2DRender.h"

class Fade;

class Result : public IGameObject
{
public:
	Result();
	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// リザルトを設定。
	/// </summary>
	/// <param name="isClear">クリアしたかどうか。falseでゲームオーバー。</param>
	void SetResult(const bool isClear)
	{
		if (isClear)
			m_resultState = enState_GameClear;
		else
			m_resultState = enState_GameOver;
	}

private:

	/// <summary>
	/// ゲームクリア時の初期化処理。
	/// </summary>
	void InitGameClear();

	/// <summary>
	/// ゲームオーバー時の初期化処理。
	/// </summary>
	void InitGameOver();

	/// <summary>
	/// 入力処理。
	/// </summary>
	void Input();

	/// <summary>
	/// ゲームクリア処理。
	/// </summary>
	void GameClear();

	/// <summary>
	/// ゲームオーバー処理。
	/// </summary>
	void GameOver();

	/// <summary>
	/// 待機時間が終了したかどうか
	/// </summary>
	/// <returns></returns>
	const bool& IsEndAnimation()
	{
		if (m_canInputTime < 0.0f) {
			return true;
		}
		else {
			m_canInputTime -= g_gameTime->GetFrameDeltaTime();
			return false;
		}
	}

private:
	//リザルトステート
	enum ResultState
	{
		enState_GameClear,
		enState_GameOver
	};

private:
	SpriteRender	m_backGroundSpriteRender;				//背景画像
	SpriteRender	m_stateSpriteRender;					//ステート文字画像
	SpriteRender	m_explainSpriteRender[2];				//詳細画像
	SpriteRender	m_cursorSpriteRender;					//カーソル
	FontRender		m_messageFontRender[4];					//スコアメッセージ
	FontRender		m_rankFontRender;						//ランク文字

	Level2DRender	m_level2DRender;						//レベル2Dレンダー

	ResultState		m_resultState = enState_GameClear;		//リザルトステート

	Fade*			m_fade = nullptr;						//フェードクラス

	Vector3			m_fontPosition[4];						//文字を出す座標

	bool			m_isWaitFadeOut = false;				//フェードしているかどうか
	int				m_cursor = 0;							//カーソル
	int				m_score[4];								//クリア時のスコア
	float			m_timer = 0.0f;							//タイマー
	float			m_alpha = 0.0f;							//透明度
	float			m_canInputTime = 0.0f;					//入力可能時間
};

