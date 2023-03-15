#pragma once

class Player;
class Mirror;

class nsK2EngineLow::Texture;

class Debug : public IGameObject
{
public:
	Debug();
	~Debug();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_stageModelRender;
	ModelRender m_boxModelRender;
	FontRender m_fontRender;

	LevelRender* m_levelRender = nullptr;

	Mirror* m_mirror = nullptr;

	PointLight m_pointLight;

	nsK2EngineLow::Texture texture[3];
	int i = 0;
};