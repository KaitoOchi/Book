#pragma once


class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender m_spriteRender;

	Vector3 m_position = Vector3(500.0f, 0.0f, 0.0f);
};