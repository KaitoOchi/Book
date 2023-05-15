#pragma once
class Game;
class Enemy;
class Gage:public IGameObject
{
public:
	Gage();
	~Gage();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 警戒度を上げる
	/// </summary>
	/// <param name="GageUp">警戒度を増やす量</param>
	void GageUp(const int GageUp, const bool isEnemy);

private:
	
	/// <summary>
	/// 警戒度を下げる
	/// </summary>
	void GageDown();

	/// <summary>
	/// 警戒度レベルを切り替える
	/// </summary>
	void VigilaceLeverChange();
	
	void Gage_ColorChange(); 

	enum EnLeverState
	{
		m_enLever_1,		//警戒度レベル１
		m_enLever_2,		//警戒度レベル２
		m_enLever_3,		//警戒度レベル３
		m_enLever_MAX,		//警戒度レベルMAX
	};

	EnLeverState m_leverState= m_enLever_1;

	EnLeverState m_GetState = m_leverState;

	std::array<SpriteRender,4>m_LeverUPRender;					//警戒度レベル画像
	SpriteRender m_baseRender;									//下に置く画像
	std::array<SpriteRender,10>m_vigilanceRender;				//警戒度画像
	bool m_isFind = false;										//発見されたかどうか
	int m_vigilanceGage = 0;									//警戒度ゲージ量
	float m_vigilanceTime = 2.0f;								//警戒度に入るクールタイム

	float m_Color=0.7f;											//警戒度カラー						

	float m_HitTime = 5.0f;
	float m_vigilaceDownTime = 2.0f;							//警戒度を下げるクールタイム

	Game* m_game = nullptr;
};

