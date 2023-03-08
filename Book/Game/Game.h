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
	ModelRender m_stageModelRender;
	PhysicsStaticObject m_demobg;


};

