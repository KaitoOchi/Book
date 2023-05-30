#include "stdafx.h"
#include "GoalSprite.h"

namespace
{
	const Vector3	GOALSPRITE_FIRST_POS = { -0.0f, 300.0f, 0.0f };		//画像の座標
	const float		GOALSPRITE_MOVESPEED = 30.0f;						//移動速度
	const float		GOALSPRITE_ENABLE_TIME = 4.5f;						//表示時間
}


GoalSprite::GoalSprite()
{

}

GoalSprite::~GoalSprite()
{

}

bool GoalSprite::Start()
{
	m_goalSpriteRender[0].Init("Assets/sprite/UI/Gauge/image_target.DDS", 231.0f, 60.0f);
	m_goalSpriteRender[1].Init("Assets/sprite/UI/Gauge/image_target2.DDS", 231.0f, 60.0f);

	for (int i = 0; i < 2; i++) {
		m_goalSpriteRender[i].SetPosition(GOALSPRITE_FIRST_POS);
		m_goalSpriteRender[i].SetScale(Vector3(1.5f, 1.5f, 1.5f));
		m_goalSpriteRender[i].Update();
	}

	m_goalPos = GOALSPRITE_FIRST_POS;

	return true;
}

void GoalSprite::Update()
{
	//時間が経過したら
	if (m_goalTimer > GOALSPRITE_ENABLE_TIME) {
		DeleteGO(this);
		return;
	}

	m_goalTimer += g_gameTime->GetFrameDeltaTime();
	m_alphaTimer += g_gameTime->GetFrameDeltaTime();

	if (m_alphaTimer > 1.0f) {
		m_alphaTimer = -0.5f;
	}


	//透過量を計算する
	float alpha	= fabsf(-pow(m_alphaTimer, 2.0f) + (2.0f * m_alphaTimer));

	//画像の設定
	m_goalSpriteRender[m_enableNum].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_goalSpriteRender[m_enableNum].Update();
}

void GoalSprite::Render(RenderContext& rc)
{
	m_goalSpriteRender[m_enableNum].Draw(rc);
}