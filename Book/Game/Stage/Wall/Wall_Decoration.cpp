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
	// ���f����ǂݍ���
	m_wallRender.Init("Assets/modelData/level_test/tkm/box.tkm", 0, 0, enModelUpAxisZ, true, true, 0);
	// �����蔻����쐬
	Wall::CreateBoxObj();
	Wall::Start();

	return true;
}

void Wall_Decoration::Render(RenderContext& rc)
{
	Wall::Render(rc);
}