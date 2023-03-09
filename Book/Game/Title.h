#pragma once
class Title :public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	SpriteRender m_titleRender;
	void Render(RenderContext &rc);
};
