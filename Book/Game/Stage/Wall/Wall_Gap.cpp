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
	Wall::Start();

	// モデルを読み込む
	m_wallRender.Init("Assets/modelData/level_test/tkm/wall_gap.tkm", 0, 0, enModelUpAxisZ, true, true, 0);

	return true;
}

void Wall_Gap::ModelLoad(int i)
{
	//switch (i) {
	//case 1:
	//	// モデルを読み込む
	//	m_wallRender.Init("Assets/modelData/level_test/tkm/wall_gap.tkm", 0, 0, enModelUpAxisZ, true, true, 0);
	//	break;
	//case 2:
	//	// モデルを読み込む
	//	m_wallRender.Init("Assets/modelData/level_test/tkm/wall2.tkm", 0, 0, enModelUpAxisZ, true, true, 0);
	//	break;
	//case 3:

	//	// モデルを読み込む
	//	m_wallRender.Init("Assets/modelData/level_test/tkm/wall3.tkm", 0, 0, enModelUpAxisZ, true, true, 0);
	//	break;
	//}
}


void Wall_Gap::Render(RenderContext& rc)
{
	Wall::Render(rc);
}