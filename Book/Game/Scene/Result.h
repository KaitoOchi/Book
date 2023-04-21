#pragma once

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
	/// <param name="vigilance">警戒度。</param>
	/// <param name="clearTime">クリアタイム。</param>
	void SetResult(const bool isClear = false, const int vigilance = 99, const float clearTime = 99.0f)
	{
		m_vigilance = vigilance;
		m_clearTime = clearTime;

		if (isClear)
			m_resultState = enState_GameClear;
		else
			m_resultState = enState_GameOver;
	}

private:
	/// <summary>
	/// ゲームクリア処理。
	/// </summary>
	void GameClear();

	/// <summary>
	/// ゲームオーバー処理。
	/// </summary>
	void GameOver();

private:
	//リザルトステート
	enum ResultState
	{
		enState_GameClear,
		enState_GameOver
	};

private:
	SpriteRender	m_backGroundSpriteRender;				//背景画像
	SpriteRender	m_cursorSpriteRender;					//カーソル

	ResultState		m_resultState = enState_GameOver;		//リザルトステート

	Fade*			m_fade = nullptr;						//フェードクラス

	int				m_vigilance = 0;						//警戒度
	int				m_cursor = 0;							//カーソル
	float			m_clearTime = 0.0f;						//クリアタイム
	float m_timer = 0.0f;
	float m_alpha = 0.0f;
};

