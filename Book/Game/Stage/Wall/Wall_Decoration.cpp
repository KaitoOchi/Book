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

	if (!m_isCreate) {
		return true;
	}

	m_physicsStaticObj.CreateFromModel(m_wallRender.GetModel(), m_wallRender.GetModel().GetWorldMatrix());

	return true;
}

void Wall_Decoration::Render(RenderContext& rc)
{
	if (!m_isCreate) {
		Wall::Render(rc);
	}
	else {
		m_wallRender.Draw(rc);
	}
}