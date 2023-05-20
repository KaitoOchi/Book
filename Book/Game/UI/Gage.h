#pragma once
class Game;
class Enemy;
#include "Wipe.h"

class Gage:public IGameObject
{
public:
	Gage();
	~Gage();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// Œx‰ú“x‚ğã‚°‚é
	/// </summary>
	/// <param name="GageUp">Œx‰ú“x‚ğ‘‚â‚·—Ê</param>
	void GageUp(const int GageUp, const bool isEnemy);

	/// <summary>
	/// ãƒ¯ã‚¤ãƒ—ã®çµ‚äº†å‡¦ç†ã€‚
	/// </summary>
	void SetWipeEnd()
	{
		m_wipe->WipeEnd();
	}

	/// <summary>
	/// ƒƒCƒv‚ÌI—¹ˆ—B
	/// </summary>
	void SetWipeEnd()
	{
		m_wipe->WipeEnd();
	}



	enum EnLeverState
	{
		m_enLever_1,		//Œx‰ú“xƒŒƒxƒ‹‚P
		m_enLever_2,		//Œx‰ú“xƒŒƒxƒ‹‚Q
		m_enLever_3,		//Œx‰ú“xƒŒƒxƒ‹‚R
		m_enLever_MAX,		//Œx‰ú“xƒŒƒxƒ‹MAX
	};

	EnLeverState m_leverState = m_enLever_1;

private:
	
	/// <summary>
	/// Œx‰ú“x‚ğ‰º‚°‚é
	/// </summary>
	void GageDown();

	/// <summary>
	/// Œx‰ú“xƒŒƒxƒ‹‚ğØ‚è‘Ö‚¦‚é
	/// </summary>
	void VigilaceLeverChange();
	
	void Gage_ColorChange(); 

	/// <summary>
	/// Œx‰ú“x‚ªMAX‚Ì‚Ìˆ—
	/// </summary>
	void Gage_MAX();

	EnLeverState m_GetState = m_leverState;

	std::array<SpriteRender,4>m_LeverUPRender;					//Œx‰ú“xƒŒƒxƒ‹‰æ‘œ
	SpriteRender m_baseRender;									//‰º‚É’u‚­‰æ‘œ
	std::array<SpriteRender,10>m_vigilanceRender;				//Œx‰ú“x‰æ‘œ
	bool m_isFind = false;										//”­Œ©‚³‚ê‚½‚©‚Ç‚¤‚©
	int m_vigilanceGage = 0;									//Œx‰ú“xƒQ[ƒW—Ê
	float m_vigilanceTime = 2.0f;								//Œx‰ú“x‚É“ü‚éƒN[ƒ‹ƒ^ƒCƒ€

	float m_Color=0.7f;											//Œx‰ú“xƒJƒ‰[						

	float m_HitTime = 5.0f;
	float m_vigilaceDownTime = 2.0f;							//Œx‰ú“x‚ğ‰º‚°‚éƒN[ƒ‹ƒ^ƒCƒ€

	Game* m_game = nullptr;
	Wipe* m_wipe = nullptr;
};

