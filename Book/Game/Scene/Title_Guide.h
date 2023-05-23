#pragma once

class Fade;
class Title;

class Title_Guide : public IGameObject
{
public:
	Title_Guide();
	~Title_Guide();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// タイトルクラスのポインタを設定。
	/// </summary>
	void SetTitlePtr(Title* title)
	{
		m_title = title;
	}

private:
	/// <summary>
	/// 入力処理。
	/// </summary>
	void Input();

	/// <summary>
	/// ステートの遷移処理。
	/// </summary>
	void StateChange();

	/// <summary>
	/// アニメーション処理。
	/// </summary>
	void Animation();

private:
	SpriteRender						m_guideBackSpriteRender;	//遊び方背景画像
	std::array< SpriteRender, 2 >		m_guideSpriteRender;		//ガイド画像
	std::array< SpriteRender, 2 >		m_buttonSpriteRender;		//ボタン画像
	std::vector< SpriteRender* >		m_sprites;					//SpriteRenderのベクター型

	Fade*								m_fade = nullptr;
	Title*								m_title = nullptr;

	Vector3								m_bezierPos[4];				//ベジェ曲線の座標
	Vector3								m_guidePos[2];				//ガイドの座標

	bool								m_isWaitState = false;		//ステートの遷移待ちかどうか
	bool								m_isSceneChange = false;	//シーンを遷移するかどうか
	bool								m_isWaitFadeOut = false;	//フェード中かどうか
	bool								m_forward = false;			//ガイド画像の全面
	int									m_cursor = 0;				//カーソル
	float								m_timer = 0.0f;				//時間
	float								m_alpha = 0.0f;				//透明度
};