#pragma once


class Logo : public IGameObject
{
public:
	Logo();
	~Logo();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender	m_logoSpriteRender;		//ƒƒS‰æ‘œ
	float			m_timer = 0.0f;			//ŠÔ
};