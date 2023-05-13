#include "stdafx.h"
#include "Wipe.h"


Wipe::Wipe()
{

}

Wipe::~Wipe()
{

}

bool Wipe::Start()
{
    m_modelRender.Init("Assets/modelData/player/player.tkm");
    m_modelRender.SetUseWipeCamera(true);
    m_modelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    m_modelRender.SetScale(Vector3(1.0f, 1.0f, 1.0f));
    m_modelRender.Update();

	return true;
}

void Wipe::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA)) {
        RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftY += 10;
    }
    if (g_pad[0]->IsTrigger(enButtonB)) {
        RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftY -= 10;
    }
}

void Wipe::Render(RenderContext& rc)
{
    m_modelRender.Draw(rc);
}