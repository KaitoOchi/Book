#pragma once


class GameUI : public IGameObject
{
public:
	GameUI();
	~GameUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	void Time();

	SpriteRender m_gageSpriteRender;
	FontRender m_timeFontRender;

	float m_timer = 130.0f;
};

