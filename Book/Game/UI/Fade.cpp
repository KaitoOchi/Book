#include "stdafx.h"
#include "Fade.h"


namespace
{
	const Vector3 LOADING_FONT_POS = Vector3(550.0f, -425.0f, 0.0f);		//ロード中文字の座標。
	const Vector3 LOADING_SPRITE_POS = Vector3(280.0f, -425.0f, 0.0f);		//ロード中画像の座標。
	const float FADE_MAX = 1.0f;											//フェードの最大値。
	const float FADE_TIPS_MAX = 2.0f;										//ヒント付きフェードの最大値。
	const float FADE_MIN = 0.0f;											//フェードの最小値。
}

Fade::Fade()
{
	//乱数を初期化。
	srand(time(0));
}

Fade::~Fade()
{

}

bool Fade::Start()
{
	//暗転時の画像
	m_spriteRender.Init("Assets/sprite/black.dds", 1920.0f, 1080.0f);

	//ローディング文字画像の設定
	m_nowLoadingSpriteRender.Init("Assets/sprite/UI/tips/now_loading.DDS", 455.0f, 53.0f);
	m_nowLoadingSpriteRender.SetPosition(LOADING_FONT_POS);
	m_nowLoadingSpriteRender.Update();

	//ローディング画像の設定
	m_loadingSpriteRender.Init("Assets/sprite/UI/tips/loading.DDS", 148.0f, 148.0f);
	m_loadingSpriteRender.SetPosition(LOADING_SPRITE_POS);
	m_loadingSpriteRender.SetScale(Vector3(0.3f, 0.3f, 0.0f));
	m_loadingSpriteRender.Update();

	//tip画像1～15を設定
	wchar_t path[32];
	char finalFilePath[32];
	for (int i = 1; i < TIPS_MAX + 1; i++) {
		
		swprintf_s(path, 255, L"Assets/sprite/UI/tips/%d.DDS", i);
		//wchar_tをcharに変換
		wcstombs(finalFilePath, path, sizeof(path));
		//tips画像の設定
		m_tipsSpriteRender[i - 1].Init(finalFilePath, 1178.0f, 755.0f);
	}
	return true;
}

void Fade::Update()
{
	switch (m_state)
	{
		//フェードイン中なら
	case enState_FadeIn:

		m_alpha -= 1.0f * g_gameTime->GetFrameDeltaTime();

		if (m_alpha <= FADE_MIN)
		{
			m_alpha = FADE_MIN;
			m_state = enState_Idle;
		}
		break;

		//フェードアウト中なら
	case enState_FadeOut:

		m_alpha += 1.0f * g_gameTime->GetFrameDeltaTime();

		if (m_alpha >= FADE_TIPS_MAX)
		{
			if (m_enableTips) {
				m_alpha = FADE_TIPS_MAX;
			}
			else {
				m_alpha = FADE_MAX;
			}
			m_state = enState_Idle;
		}
		break;

	case enState_Idle:
		break;
	}

	if (m_alpha < FADE_MIN) {
		return;
	}

	SpriteUpdate();
}

void Fade::SpriteUpdate()
{
	Vector4 alpha = Vector4(1.0f, 1.0f, 1.0f, m_alpha);

	//背景画像を設定
	m_spriteRender.SetMulColor(alpha);
	m_spriteRender.Update();

	//tips画像を設定
	if (m_enableTips) {
		m_tipsSpriteRender[m_spriteNum].SetMulColor(alpha);
		m_tipsSpriteRender[m_spriteNum].Update();

		//ローディング文字の設定
		m_nowLoadingSpriteRender.SetMulColor(alpha);
		m_nowLoadingSpriteRender.Update();

		//ローディング画像の設定
		m_rotTimer += g_gameTime->GetFrameDeltaTime();
		Quaternion rot;
		rot.SetRotationZ(Math::DegToRad(m_rotTimer * CIRCLE_ROADING_ROT));
		m_loadingSpriteRender.SetRotation(rot);
		m_loadingSpriteRender.SetMulColor(alpha);
		m_loadingSpriteRender.Update();
	}
	else {
		m_tipsSpriteRender[m_spriteNum].SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		m_tipsSpriteRender[m_spriteNum].Update();
	}
}

void Fade::Render(RenderContext& rc)
{
	//フェードが終了しているなら
	if (m_alpha < FADE_MIN) {
		return;
	}

	//背景画像の描画
	m_spriteRender.Draw(rc);

	//ヒント画像を出すなら
	if (m_enableTips) {

		//ヒント画像の描画
		m_tipsSpriteRender[m_spriteNum].Draw(rc);

		//ローディング文字の描画
		m_nowLoadingSpriteRender.Draw(rc);

		//ローディング画像の描画
		m_loadingSpriteRender.Draw(rc);
	}
}