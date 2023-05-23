#include "stdafx.h"
#include "Title_Guide.h"

#include "Fade.h"
#include "Title.h"

namespace
{
	const Vector3 BEZIER_POS[4] = { Vector3(-25.0f, 25.0f, 0.0f),
									Vector3(25.0f, 25.0f, 0.0f), 
									Vector3(25.0f, -25.0f, 0.0f),
									Vector3(-25.0f, -25.0f, 0.0f) };		//ベジェ曲線の座標
}

Title_Guide::Title_Guide()
{

}

Title_Guide::~Title_Guide()
{
	for (auto& sprites : m_sprites)
	{
		m_sprites.pop_back();
	}
	m_sprites.shrink_to_fit();
}

bool Title_Guide::Start()
{
	//ガイド背景画面の設定
	m_guideBackSpriteRender.Init("Assets/sprite/UI/guide/guide_all2.DDS", 1920.0f, 1080.0f);
	m_guideBackSpriteRender.Update();
	m_sprites.push_back(&m_guideBackSpriteRender);

	//ガイド画面の設定
	m_guideSpriteRender[0].Init("Assets/sprite/UI/guide/guide_playguide.DDS", 1403.0f, 637.0f);
	m_guideSpriteRender[1].Init("Assets/sprite/UI/guide/rule.DDS", 1141.0f, 541.0f);
	for (int i = 0; i < 2; i++) {
		m_guideSpriteRender[i].SetPosition(BEZIER_POS[i * 2]);
		m_guideSpriteRender[i].Update();
		m_sprites.push_back(&m_guideSpriteRender[i]);
	}

	//ボタン画像の設定
	m_buttonSpriteRender[0].Init("Assets/sprite/UI/button/text_Abutton.DDS", 287.0f, 152.0f);
	m_buttonSpriteRender[1].Init("Assets/sprite/UI/button/text_Bbutton.DDS", 287.0f, 152.0f);
	for (int i = 0; i < 2; i++) {
		m_buttonSpriteRender[i].SetPosition(Vector3(-825.0f, -375.0f - (i * 50.0f), 0.0f));
		m_buttonSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_buttonSpriteRender[i].SetScale(Vector3(0.75f, 0.75f, 0.0f));
		m_buttonSpriteRender[i].Update();
		m_sprites.push_back(&m_buttonSpriteRender[i]);
	}

	//フェードの処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Title_Guide::Update()
{
	//シーンの遷移
	if (m_isWaitState) {
		StateChange();
	}
	else {
		Input();
	}
}

void Title_Guide::Input()
{
	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		if (m_cursor != 0) {
			m_isSceneChange = true;
		}
		else {
			m_cursor++;
		}
		m_timer = 1.0f;
		m_isWaitState = true;
		//可能ならSEを再生
		m_title->IsCanPlaySound(true);

	}
	//Bボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonB))
	{
		if (m_cursor == 1) {
			m_cursor = 0;
		}
		else {
			m_isSceneChange = true;

		}
		m_timer = 1.0f;
		m_isWaitState = true;
		//可能ならSEを再生
		m_title->IsCanPlaySound(false);
	}
}

void Title_Guide::StateChange()
{

	if (m_isWaitFadeOut) {
		//フェードし終えたら
		if (!m_fade->IsFade()) {
			//タイトルクラスへ戻る
			m_title->SetActive(true);
			DeleteGO(this);
		}
		return;
	}
	else {
		//アニメーションが終了したら
		if (m_timer < 0.0f) {

			if (m_isSceneChange) {
				m_isWaitFadeOut = true;
				m_fade->StartFadeOut();
			}
			else {
				m_isWaitState = false;
				m_timer = 0.0f;
			}
		}
		else {
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}
	}

	if (m_isSceneChange) {
		//画像の透明度を変更
		for (auto& sprites : m_sprites)
		{
			sprites->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(m_timer)));
			sprites->Update();
		}
	}
	else {
		//アニメーションの再生
		Animation();
	}
}

void Title_Guide::Animation()
{
	m_bezierPos[0].Lerp(m_timer, BEZIER_POS[0], BEZIER_POS[1]);
	m_bezierPos[1].Lerp(m_timer, BEZIER_POS[1], BEZIER_POS[2]);
	m_bezierPos[2].Lerp(m_timer, BEZIER_POS[2], BEZIER_POS[3]);
	m_bezierPos[3].Lerp(m_timer, BEZIER_POS[3], BEZIER_POS[0]);
	m_guidePos[0].Lerp(m_timer, m_bezierPos[0], m_bezierPos[1]);
	m_guidePos[1].Lerp(m_timer, m_bezierPos[2], m_bezierPos[3]);


	if (m_cursor == 1) {
		m_guideSpriteRender[0].SetPosition(m_guidePos[0]);
		m_guideSpriteRender[0].Update();
		m_guideSpriteRender[1].SetPosition(m_guidePos[1]);
		m_guideSpriteRender[1].Update();
	}
	else {
		m_guideSpriteRender[0].SetPosition(m_guidePos[1]);
		m_guideSpriteRender[0].Update();
		m_guideSpriteRender[1].SetPosition(m_guidePos[0]);
		m_guideSpriteRender[1].Update();
	}

	//画像の全面を反転させる
	if (m_timer < 0.5f && m_timer > 0.49f) {
		m_forward = !m_forward;
	}
}

void Title_Guide::Render(RenderContext& rc)
{
	//背景画像の描画
	m_guideBackSpriteRender.Draw(rc);

	//ガイド画像の描画
	if (m_forward) {
		m_guideSpriteRender[0].Draw(rc);
		m_guideSpriteRender[1].Draw(rc);
	}
	else {
		m_guideSpriteRender[1].Draw(rc);
		m_guideSpriteRender[0].Draw(rc);
	}

	//ボタン画像の描画
	for (int i = 0; i < 2; i++) {
		m_buttonSpriteRender[i].Draw(rc);
	}

}