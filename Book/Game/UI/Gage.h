#pragma once
class Gage:public IGameObject
{
public:
	Gage();
	~Gage();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	void GageUp(int GageUp);
	SpriteRender m_LeverUPRender[4];
	SpriteRender m_baseRender;
	SpriteRender m_vigilanceRender[10];
	float m_vigilanceTime = 2.0f;
	int m_vigilanceGage = 1;
	int m_Gitgage = 0;

	float m_redColor=0.7f;
	float m_greenColor = 0;
	float m_blueColor=1.0f;

	float testTimer = 1.0f;
};

