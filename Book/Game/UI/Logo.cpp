#include "stdafx.h"
#include "Logo.h"

#include "Title.h"


Logo::Logo()
{

}

Logo::~Logo()
{

}

bool Logo::Start()
{
	m_logoSpriteRender.Init("Assets/sprite/UI/title/image_rogo.DDS", 216.0f, 129.0f);
	m_logoSpriteRender.SetScale(Vector3(1.5f, 1.5f, 0.0f));

	return true;
}

void Logo::Update()
{
	m_timer += g_gameTime->GetFrameDeltaTime();

	//ŠÔ‚ªŒo‰ß‚µ‚½‚ç
	if (m_timer > 3.0f) {
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}

	float alpha = ((pow(m_timer, 2.0f) * -0.75f) + (2.0f * m_timer));
	alpha = max(min(alpha, 1.0f), 0.0f);

	//ƒƒS‰æ‘œ‚Ìİ’è
	m_logoSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_logoSpriteRender.Update();
}

void Logo::Render(RenderContext& rc)
{
	m_logoSpriteRender.Draw(rc);
}