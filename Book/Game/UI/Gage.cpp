#include "stdafx.h"
#include "Gage.h"
namespace
{
	const float		VIGILANCE_TIME_MAX = 2.0f;							//警戒値の最大時間
	const float		BASEYSIZE = 154.0f;									//base縦の大きさ
	const float     BASEXSIZE = 553.0f;									//base横の大きさ
	const float		VIGIRANCE_XSIZE = 67.0f;							//警戒度の縦の大きさ
	const float		VIGIRANCE_YSIZE = 80.0f;							//警戒度の横の大きさ
	const int		MAXGAGECOUNT = 10;
	const int		MAXLEVERCOUNT = 4;
}
Gage::Gage()
{

}
Gage::~Gage()
{

}
bool Gage::Start()
{
	//基盤の更新
	m_baseRender.Init("Assets/sprite/UI/Gauge/base.DDS", BASEXSIZE, BASEYSIZE);
	m_baseRender.SetPosition(Vector3(230.0f,0.0f,0.0f));
	m_baseRender.Update();
	//ゲージの作成
	for (int i = 0; i < MAXGAGECOUNT; i++)
	{
		m_vigilanceRender[i].Init("Assets/sprite/UI/Gauge/gaugeCount.DDS", VIGIRANCE_XSIZE, VIGIRANCE_YSIZE);
		m_vigilanceRender[i].SetPosition(Vector3(40.0f * i, 0.0f, 0.0f));
		m_redColor += 0.005 * i;
		
		m_vigilanceRender[i].SetMulColor(Vector4(m_redColor, 0.0f, 0.0f, 1.0f));
		m_vigilanceRender[i].Update();
	}

	//レベルの作成
	m_LeverUPRender[0].Init("Assets/sprite/UI/Gauge/1.DDS", BASEXSIZE, BASEYSIZE);
	m_LeverUPRender[1].Init("Assets/sprite/UI/Gauge/2.DDS", BASEXSIZE, BASEYSIZE);
	m_LeverUPRender[2].Init("Assets/sprite/UI/Gauge/3.DDS", BASEXSIZE, BASEYSIZE);
	m_LeverUPRender[3].Init("Assets/sprite/UI/Gauge/MAX.DDS", BASEXSIZE, BASEYSIZE);
	for (int i = 0; i < MAXLEVERCOUNT ; i++)
	{
		m_LeverUPRender[i].SetPosition(Vector3(240.0f, 0.0f, 0.0f));
		m_LeverUPRender[i].Update();
	}
	

	


	return true;
}

void Gage::Update()
{

}
void Gage::GageUp(int GageUp)
{
	//クールダウンがまだなら
	if (m_vigilanceTime > 0.0f) 
	{
		return;
	}

	m_vigilanceGage += GageUp;
	if (m_vigilanceGage != m_Gitgage)
	{
		m_Gitgage = m_vigilanceGage;
		//VigilanceChange();
	}
	m_vigilanceTime = VIGILANCE_TIME_MAX;
}
void Gage::Render(RenderContext& rc)
{
	m_baseRender.Draw(rc);
	for (int i = 0; i < MAXGAGECOUNT; i++)
	{
		m_vigilanceRender[i].Draw(rc);
	}
	for (int i = 0; i < MAXLEVERCOUNT; i++)
	{
		m_LeverUPRender[i].Draw(rc);
	}
	
}