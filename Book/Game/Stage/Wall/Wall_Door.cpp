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
	// ���f����ǂݍ���
	m_wallRender.Init("Assets/modelData/level_test/tkm/door.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
	// �����蔻����쐬
	Wall::CreateBoxObj();
	Wall::Start();

	return true;
}

void Wall_Door::Render(RenderContext&rc)
{
	Wall::Render(rc);
}