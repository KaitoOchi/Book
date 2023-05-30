#include "stdafx.h"
#include "Wall_Duct.h"

Wall_Duct::Wall_Duct()
{

}

Wall_Duct::~Wall_Duct()
{

}

bool Wall_Duct::Start()
{
	// モデルを読み込む
	m_wallRender.Init("Assets/modelData/level_test/tkm/duct.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);

	Wall::Start();

	return true;
}

void Wall_Duct::Render(RenderContext& rc)
{
	Wall::Render(rc);
}