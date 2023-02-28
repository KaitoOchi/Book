#pragma once


class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
	SpriteRender m_spriteRender;
	FontRender m_fontRender;

	Vector3 m_position = Vector3(500.0f, 0.0f, 0.0f);
};

