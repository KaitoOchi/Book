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
	m_wallRender.Init("Assets/modelData/level_test/tkm/box.tkm", 0, 0, enModelUpAxisZ, 1, 1);
	// �����蔻����쐬
	Wall::CreateBoxObj();
	Wall::Start();

	return true;
}

void Wall_Door::Render(RenderContext&rc)
{
	Wall::Render(rc);
}