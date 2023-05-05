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

	return true;
}

void Wall_Gap::ModelLoad(int i)
{
	switch (i) {
	case 1:
		// ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
		m_wallRender.Init("Assets/modelData/level_test/tkm/wall1.tkm", 0, 0, enModelUpAxisZ, true, true, 0);
		break;
	case 2:
		// ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
		m_wallRender.Init("Assets/modelData/level_test/tkm/wall2.tkm", 0, 0, enModelUpAxisZ, true, true, 0);
		break;
	case 3:

		// ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
		m_wallRender.Init("Assets/modelData/level_test/tkm/wall3.tkm", 0, 0, enModelUpAxisZ, true, true, 0);
		break;
	}
}

void Wall_Gap::Render(RenderContext& rc)
{
	Wall::Render(rc);
}