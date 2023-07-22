#include "stdafx.h"
#include "Wall_Door.h"

Wall_Door::Wall_Door()
{

}

Wall_Door::~Wall_Door()
{

}

bool Wall_Door::Start()
{
	// モデルを読み込む
	m_wallRender.Init("Assets/modelData/level_test/tkm/door.tkm", 0, 0, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK, false);
	// 当たり判定を作成
	Wall::CreateBoxObj();
	Wall::Start();

	return true;
}

void Wall_Door::Render(RenderContext&rc)
{
	Wall::Render(rc);
}