#pragma once

class PlayerManagement;
class GameUI : public IGameObject	
{
public:
	GameUI();
	~GameUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Vigilance(int GageUp);
private:
	void Time();
	void ChangeGage();
	void VigilanceChange();
	SpriteRender m_gageFrameSpriteRender;
	SpriteRender m_gageSpriteRender;
	SpriteRender m_vigilanceRender;
	FontRender m_timeFontRender;
	PlayerManagement* m_playerManagement;
	float m_timer = 0.0f;
	float m_vigilanceTime = 0.0f;
	float m_gage = 0.0f;
	int m_vigilanceGage=1;
	int m_Gitgage = 0;

};

