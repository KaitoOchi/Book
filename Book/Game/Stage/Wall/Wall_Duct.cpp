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
	// ÉÇÉfÉãÇì«Ç›çûÇﬁ
	m_wallRender.Init("Assets/modelData/level_test/tkm/duct.tkm", 0, 0, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK, true);

	Wall::Start();

	return true;
}

void Wall_Duct::Render(RenderContext& rc)
{
	Wall::Render(rc);
}