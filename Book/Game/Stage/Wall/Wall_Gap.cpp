#include "stdafx.h"
#include "Wall_Gap.h"

Wall_Gap::Wall_Gap()
{

}

Wall_Gap::~Wall_Gap()
{

}

bool Wall_Gap::Start()
{
	// モデルを読み込む
	m_wallRender.Init("Assets/modelData/level_test/tkm/wall_gap.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);

	Wall::Start();

	return true;
}

void Wall_Gap::Render(RenderContext& rc)
{
	Wall::Render(rc);
}