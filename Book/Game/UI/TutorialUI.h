#pragma once

class GameUI;
class TutorialUI:public IGameObject
{
public:
	TutorialUI();
	~TutorialUI();

private:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// チュートリアルの画像の描画状態を設定。
	/// </summary>
	/// <param name="state1">trueなら描画する。</param>
	void SetTutorialDrawState(const bool state)
	{
		m_tutorialDrawState = state;
	}

	/// <summary>
	/// 現在の描画状態を取得する。
	/// </summary>
	/// <returns></returns>
	const bool GetTutorialDrawState()
	{
		return m_tutorialDrawState;
	}

private:
	SpriteRender		m_guideSpriteRender;			//チュートリアルガイドのテキスト画像
	bool				m_tutorialDrawState = true;		//チュートリアルのテキスト画像を描画するかどうか
};

