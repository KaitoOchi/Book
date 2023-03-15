#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
}

BackGround::~BackGround()
{
}

bool BackGround::Start()
{
	// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_ModelRender.Init("Assets/modelRender/debugStage/debug.tkm");

	return true;
}

void BackGround::Render(RenderContext& rc)
{
	m_ModelRender.Draw(rc);
}