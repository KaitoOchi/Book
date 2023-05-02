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
	// ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	m_wallRender.Init("Assets/modelData/level_test/tkm/wall1.tkm", 0, 0, enModelUpAxisZ, 1, 1);

	Wall::Start();

	return true;
}

void Wall_Gap::Render(RenderContext& rc)
{
	Wall::Render(rc);
}