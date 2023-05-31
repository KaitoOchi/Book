#include "stdafx.h"
#include "Painting.h"

Painting::Painting()
{

}

Painting::~Painting()
{

}

bool Painting::Start()
{
	//絵画モデルの設定
	m_modelRender.Init("Assets/modelData/level_test/tkm/painting.tkm", 0, 0, enModelUpAxisZ, false, true);
	m_modelRender.Update();

	//テクスチャを変更
	m_modelRender.GetModel().ChangeAlbedoMap("",
		m_texture
		);

	//枠モデルの設定
	m_frameModelRender.Init("Assets/modelData/level_test/tkm/frame.tkm");
	m_frameModelRender.Update();

	return true;
}

void Painting::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_frameModelRender.Draw(rc);
}