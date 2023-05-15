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
	std::array<SpriteRender,4> m_countDownSpriteRender;		//カウントダウン画像
	float			m_timer = 1.0f;					//タイマー
	int				m_count = 4;					//カウント
};