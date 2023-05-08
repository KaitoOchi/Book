#pragma once
#include "level2DRender/Level2DRender.h"
#include "GameManager.h"

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

	/// <summary>
	/// 終了時間を設定。
	/// </summary>
	/// <param name="time"></param>
	void SetTime(const float time)
	{
		m_score[0] = time;
	}

private:
	/// <summary>
	/// スコアの計算処理。
	/// </summary>
	void InitScore();

	/// <summary>
	/// スプライトの計算処理。
	/// </summary>
	void InitSprite();

	/// <summary>
	/// 入力処理。
	/// </summary>
	void Input();

	/// <summary>
	/// アニメーション処理。
	/// </summary>
	void Animation();

	/// <summary>
	/// 待機時間が終了したかどうか
	/// </summary>
	/// <returns></returns>
	const bool IsEndAnimation()
	{
		if (m_canInputTime < 0.0f) {
			return false;
		}
		else if (m_canInputTime < 0.5f && m_canInputTime > 0.4f) {
			//ドラムロール音を出す
			m_sound = NewGO<SoundSource>(0);
			m_sound->Init(4);
			m_sound->Play(false);
			m_sound->SetVolume(GameManager::GetInstance()->GetSFX());
			m_canInputTime = 0.3f;
			return true;
		}

		m_canInputTime -= g_gameTime->GetFrameDeltaTime();
		return true;
	}

private:
	//リザルトステート
	enum ResultState
	{
		enState_GameOver,
		enState_GameClear
	};

private:
	SpriteRender	m_backGroundSpriteRender;				//背景画像
	SpriteRender	m_stateSpriteRender;					//ステート文字画像
	SpriteRender	m_explainSpriteRender[2];				//詳細画像
	SpriteRender	m_cursorSpriteRender;					//カーソル
	SpriteRender	m_rankSpriteRender;						//ランク画像
	SpriteRender	m_failedSpriteRender;					//failed文字
	FontRender		m_messageFontRender[4];					//スコアメッセージ
	FontRender		m_scoreFontRender;						//スコアフォント
	Level2DRender	m_level2DRender;						//レベル2Dレンダー
	ResultState		m_resultState = enState_GameOver;		//リザルトステート
	Fade*			m_fade = nullptr;						//フェードクラス
	SoundSource*	m_sound = nullptr;						//サウンドクラス
	Vector3			m_fontPosition[4];						//文字を出す座標
	bool			m_isWaitFadeOut = false;				//フェードしているかどうか
	bool			m_isDram = false;						//ドラム音を出したかどうか
	int				m_cursor = 0;							//カーソル
	int				m_score[4];								//クリア時のスコア
	float			m_timer = 0.0f;							//タイマー
	float			m_alpha = 0.0f;							//透明度
	float			m_canInputTime = 0.0f;					//入力可能時間
};

