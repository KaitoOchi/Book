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
	// ���f����ǂݍ���
	m_wallRender.Init("Assets/modelData/level_test/tkm/box.tkm", 0, 0, enModelUpAxisZ, 1, 1);
	// �����蔻����쐬
	Wall::CreateBoxObj();
	Wall::Start();

	return true;
}

void Wall_Normal::Render(RenderContext& rc)
{
	Wall::Render(rc);
}