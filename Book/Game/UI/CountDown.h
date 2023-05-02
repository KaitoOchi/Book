#pragma once


class CountDown : public IGameObject
{
public:
	CountDown();
	~CountDown();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender	m_countDownSpriteRender[4];		//カウントダウン画像
	float			m_timer = 0.0f;					//タイマー
	int				m_count = 3;					//カウント
};