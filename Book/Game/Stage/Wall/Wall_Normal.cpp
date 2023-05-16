#include "stdafx.h"
#include "Wall_Normal.h"

Wall_Normal::Wall_Normal()
{

}

Wall_Normal::~Wall_Normal()
{

}

bool Wall_Normal::Start()
{
	// モデルを読み込む
	m_wallRender.Init("Assets/modelData/level_test/tkm/box.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
	// 当たり判定を作成
	Wall::CreateBoxObj();
	Wall::Start();

	return true;
}

void Wall_Normal::Render(RenderContext& rc)
{
	Wall::Render(rc);
}