#include "stdafx.h"
#include "Logo.h"

#include "Title.h"


namespace
{
	const float SPRITE_SCALE = 1.5f;		//拡大率。
	const float LOGO_TIME = 3.0f;			//ロゴの表示時間。
	const float ANIM_CURVE_SPEED = 0.75;	//アニメーション速度。
	const float ALPHA_MIN = 0.0f;			//透明度の最小値。
	const float ALPHA_MAX = 1.0f;			//透明度の最大値。
}

Logo::Logo()
{

}

Logo::~Logo()
{

}

bool Logo::Start()
{
	//ロゴ画像の設定
	m_logoSpriteRender.Init("Assets/sprite/UI/title/image_rogo.DDS", 216.0f, 129.0f);
	m_logoSpriteRender.SetScale(Vector3(SPRITE_SCALE, SPRITE_SCALE, 0.0f));

	return true;
}

void Logo::Update()
{
	m_timer += g_gameTime->GetFrameDeltaTime();

	//時間が経過したら
	if (m_timer > LOGO_TIME) {
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}

	float alpha = ((pow(m_timer, 2.0f) * -ANIM_CURVE_SPEED) + (2.0f * m_timer));
	alpha = max(min(alpha, ALPHA_MAX), ALPHA_MIN);

	//ロゴ画像の設定
	m_logoSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_logoSpriteRender.Update();
}

void Logo::Render(RenderContext& rc)
{
	m_logoSpriteRender.Draw(rc);
}