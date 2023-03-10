#pragma once


class Debug;

class Mirror : public IGameObject
{
public:
	Mirror();
	~Mirror();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	Model m_model;
	RenderTarget offscreenRenderTarget;

	Debug* m_debug = nullptr;
};

