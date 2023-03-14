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
	ModelRender m_boxModelRender;
	ModelRender m_modelRender;
	RenderTarget offscreenRenderTarget;

	Debug* m_debug = nullptr;
};

