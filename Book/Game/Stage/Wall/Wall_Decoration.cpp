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

void Wall_Decoration::Render(RenderContext& rc)
{
	Wall::Render(rc);
}