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
    m_modelRender.Init("Assets/modelData/enemy/enemy_charge.tkm");
    m_modelRender.SetPosition(Vector3(10.0f, 50.0f, 10.0f));
    m_modelRender.SetScale(Vector3(10.0f, 10.0f, 10.0f));
    m_modelRender.Update();

	return true;
}

void Wipe::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA)) {
        RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftY += 10;
    }
}

void Wipe::Render(RenderContext& rc)
{

}