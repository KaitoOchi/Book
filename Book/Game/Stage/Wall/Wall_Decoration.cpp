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
	Wall::Start();
	return true;
}

void Wall_Decoration::Load_Model(int num)
{
	switch (num) {
	case 1:
		m_wallRender.Init("Assets/modelData/level_test/tkm/decoration.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
		break;
	case 2:
		m_wallRender.Init("Assets/modelData/level_test/tkm/decoration_2.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
		break;
	case 3:
		m_wallRender.Init("Assets/modelData/level_test/tkm/decoration_3.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
		break;
	case 4:
		m_wallRender.Init("Assets/modelData/level_test/tkm/decoration_4.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
		break;
	}

	m_wallRender.SetScale({ 50.0f, 50.0f, 50.0f });
}

void Wall_Decoration::Render(RenderContext& rc)
{
	Wall::Render(rc);
}