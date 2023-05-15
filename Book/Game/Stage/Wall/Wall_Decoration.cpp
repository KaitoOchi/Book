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
	// ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	m_wallRender.Init("Assets/modelData/level_test/tkm/decoration.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
	m_wallRender.SetScale({ 50.0f, 50.0f, 50.0f });
	// “–‚½‚è”»’è‚ğì¬
	Wall::CreateBoxObj();
	Wall::Start();

	return true;
}

void Wall_Decoration::Render(RenderContext& rc)
{
	Wall::Render(rc);
}