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
    m_modelRender.Init("Assets/modelData/player/player.tkm", 0, 0, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK, true);
    m_modelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    m_modelRender.SetScale(Vector3(1.0f, 1.0f, 1.0f));
    m_modelRender.Update();

    a.Init("Assets/modelData/player/test.tkm", 0, 0, enModelUpAxisZ, false, false, 4, D3D12_CULL_MODE_BACK, false);
    a.SetPosition(Vector3(0.0f, 200.0f, 0.0f));
    a.Update();

	return true;
}

void Wipe::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA)) {
        num += 100;

        RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftY += 10;
        RenderingEngine::GetInstance()->GetWipeViewPort().Height += 10;
    }
    if (g_pad[0]->IsTrigger(enButtonB)) {
        RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftX += 10;
        RenderingEngine::GetInstance()->GetWipeViewPort().Width += 10;
    }
}

void Wipe::Render(RenderContext& rc)
{
    m_modelRender.Draw(rc);

    a.Draw(rc);
}