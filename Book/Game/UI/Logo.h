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
	SpriteRender	m_logoSpriteRender;		//���S�摜
	float			m_timer = 0.0f;			//����
};