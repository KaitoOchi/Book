#include "stdafx.h"
#include "Wall_Decoration.h"

Wall_Decoration::Wall_Decoration()
{

}

Wall_Decoration::~Wall_Decoration()
{

}

bool Wall_Decoration::Start()
{
	// モデルを読み込む
	m_wallRender.Init("Assets/modelData/level_test/tkm/decoration.tkm", 0, 0, enModelUpAxisZ, true, true, 0);
	m_wallRender.SetScale({ 50.0f, 50.0f, 50.0f });
	// 当たり判定を作成
	Wall::CreateBoxObj();
	Wall::Start();

	return true;
}

void Wall_Decoration::Render(RenderContext& rc)
{
	Wall::Render(rc);
}