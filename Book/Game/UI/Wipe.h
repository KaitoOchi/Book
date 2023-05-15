#pragma once

class Wipe : public IGameObject
{
public:
	Wipe();
	~Wipe();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
	ModelRender a;
	int num = 0;
};