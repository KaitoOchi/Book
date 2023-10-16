#include "stdafx.h"
#include "TutorialUI.h"
#include "GameUI.h"

TutorialUI::TutorialUI()
{
}

TutorialUI::~TutorialUI()
{
}

bool TutorialUI::Start()
{
	//チュートリアルのテキスト画像
	m_guideSpriteRender.Init("Assets/sprite/UI/Gauge/TutorialGuide.DDS", 714.0f, 66.0f);
	m_guideSpriteRender.SetScale(Vector3(0.7f, 0.7f, 0.0f));
	m_guideSpriteRender.SetPosition(Vector3(0.0f, -200.0f, 0.0f));
	m_guideSpriteRender.Update();

	return true;
}

void TutorialUI::Update()
{
	if (m_tutorialDrawState == false) {
		// 遷移する
		NewGO<GameUI>(0, "gameUI");
		// 自身を削除する
		DeleteGO(this);
	}
}

void TutorialUI::Render(RenderContext& rc)
{
	//チュートリアル画像の描画
	if (m_tutorialDrawState == true) {
		m_guideSpriteRender.Draw(rc);
	}
}
