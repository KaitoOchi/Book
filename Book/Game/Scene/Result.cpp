#include "stdafx.h"
#include "Result.h"

#include "Fade.h"
#include "Title.h"
#include "Game.h"

namespace
{
	const char		RANK[4] = { 'A', 'B', 'C', 'D' };						//ランク一覧
	const wchar_t* GET_TREASURE_TEXT[2] = {L"    FAILED", L" SUCCUESS!" };	//宝を取得したかどうかのテキスト
	const int		SCORE_RANK[4] = { 100000, 80000, 60000, 50000 };				//ランクの条件
	const float		CAN_INPUT = 6.0f;										//入力可能時間
	const float		ENABLE_TIME[5] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };		//表示可能になる時間
	const Vector3	GAMEOVER_SPRITE_POS = { -150.0f, 0.0f, 0.0f };			//ゲームオーバー画像の座標
}

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	InitSprite();

	InitScore();

	//待機時間の設定
	m_canInputTime = CAN_INPUT;

	//フェードの処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Result::InitScore()
{
	//リザルトを保存
	m_score[1] = m_resultState;
	m_score[2] = GameManager::GetInstance()->GetSearchNum();
	//m_score[2] = 5;
	m_score[3] = (m_score[0] * 100) +
		(m_score[1] * 50000) +
		(m_score[2] * -500);

	char rank = 'E';
	//スコアからランクを設定
	for (int i = 0; i < 4; i++) {
		//スコアのほうが上なら
		if (m_score[3] > SCORE_RANK[i]) {
			rank = RANK[i];
			break;
		}
	}

	//ファイルパスにランクを合わせる
	wchar_t path[255];
	swprintf_s(path, 255,
		L"Assets/sprite/UI/result/rank/%c.DDS",
		rank
	);
	char finalFilePath[100];
	wcstombs(finalFilePath, path, sizeof(path));

	//ランク表示の設定
	m_rankSpriteRender.Init(finalFilePath, 306.0f, 312.0f);
	m_rankSpriteRender.SetPosition(Vector3(250.0f, -150.0f, 0.0f));
	m_rankSpriteRender.Update();

	int m = m_score[0] / 60;
	int s = m_score[0] % 60;

	//スコア文字の設定
	for (int i = 0; i < 4; i++) {
		//スコアをテキストに変換
		wchar_t text[255];
		switch (i) {
		case 0:
			swprintf_s(text, 255,
				L"      %d:%02d",
				m,
				s
			);
			break;

		case 1:
			swprintf_s(text, 255,
				L"%s",
				GET_TREASURE_TEXT[m_resultState]
			);
			break;

		default:
			swprintf_s(text, 255,
				L"%10d",
				m_score[i]
			);
			break;
		}
		//スコア文字を設定
		m_messageFontRender[i].SetText(text);
		m_messageFontRender[i].SetPosition(m_fontPosition[i]);
		m_messageFontRender[i].SetPivot(Vector2(1.0f, 0.5f));
		m_messageFontRender[i].SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_messageFontRender[i].SetShadowParam(true, 1.5, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//金額の設定
	wchar_t text[255];
	swprintf_s(text, 255,
		L"$%d",
		m_score[3]
	);
	m_scoreFontRender.SetText(text);
	m_scoreFontRender.SetPosition(Vector3(425.0f, -225.0f, 0.0f));
	m_scoreFontRender.SetScale(2.0f);
	m_scoreFontRender.SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	m_scoreFontRender.SetShadowParam(true, 0.5f, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Result::InitSprite()
{
	int lightNum = 0;

	//背景画像の設定
	m_backGroundSpriteRender.Init("Assets/sprite/UI/result/base.DDS", 1920.0f, 1080.0f);

	//ボタンの設定
	m_cursorSpriteRender.Init("Assets/sprite/UI/title/button.DDS", 640.0f, 150.0f);
	m_cursorSpriteRender.SetPosition(Vector3(0.0f, -400.0f, 0.0f));
	m_cursorSpriteRender.Update();

	//レベルのデータを使用してタイトル画像を読み込む
	m_level2DRender.Init("Assets/level2D/result.casl", [&](Level2DObjectData& objData) {
		//名前が一致していたら。
		if (objData.EqualObjectName("missionComplete!") == true) {
			//文字画像を設定
			if (m_resultState == enState_GameClear) {
				m_stateSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
				m_stateSpriteRender.SetPosition(objData.position);
			}
			else {
				m_stateSpriteRender.Init("Assets/sprite/UI/gameOver/gameover.DDS", 516.0f, 138.0f);
				m_stateSpriteRender.SetPosition(objData.position + GAMEOVER_SPRITE_POS);
			}
			m_stateSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("result_rank") == true) {
			//ランク画像を設定
			m_explainSpriteRender[0].Init(objData.ddsFilePath, objData.width, objData.height);
			m_explainSpriteRender[0].SetPosition(objData.position);

			Quaternion rot;
			rot.SetRotationZ(Math::DegToRad(-30.0f));
			//失敗時の画像を設定
			m_failedSpriteRender.Init("Assets/sprite/UI/result/failed!.DDS", 497.0f, 170.0f);
			m_failedSpriteRender.SetPosition(objData.position);
			m_failedSpriteRender.SetRotation(rot);
			m_failedSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("result_text") == true) {
			//スコア画像を設定
			m_explainSpriteRender[1].Init(objData.ddsFilePath, objData.width, objData.height);
			m_explainSpriteRender[1].SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualObjectName("light") == true) {
			//スコアを表示する位置を設定
			m_fontPosition[lightNum] = objData.position;
			lightNum++;
			return true;
		}
		return false;
		});

	for (int i = 0; i < 2; i++) {
		m_explainSpriteRender[i].SetScale(Vector3(0.9f, 0.9f, 0.0f));
		m_explainSpriteRender[i].Update();
	}
}

void Result::Update()
{
	if (!IsEndAnimation()) {
		return;
	}

	//フェードの待機時間
	if (m_isWaitFadeOut) {
		//フェードをし終えたら
		if (!m_fade->IsFade()) {
			NewGO<Title>(0, "title");
			DeleteGO(this);
		}
		return;
	}
	else {
		Input();
	}

	//時間の処理
	m_timer += g_gameTime->GetFrameDeltaTime();

	GameClear();
}

void Result::Input()
{
	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA)) {

		//表示が全て完了していなら
		if (m_timer < ENABLE_TIME[4]) {
			//表示をスキップする
			m_timer = ENABLE_TIME[4];
		}
		else {
			//フェードを始める
			m_isWaitFadeOut = true;
			m_fade->StartFadeOut();
		}
	}
}

void Result::GameClear()
{
	float time = m_timer - ENABLE_TIME[4];

	if (time > 2.0f) {
		m_timer = ENABLE_TIME[4];
	}

	// -t^2 + 2t
	m_alpha = fabsf(-pow((time - 0.5f), 1.0f) + (2 * (time - 0.5f)));
	m_alpha = min(m_alpha, 1.0f);

	//ボタンを点滅させる
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_cursorSpriteRender.Update();

	if (m_timer > 5.0f && m_timer < 6.0f && m_cursor == 0) {
		//ランクを表示する
		m_rankSpriteRender.SetScale(Vector3(1.0f, 1.0f, 1.0f) * (7.0f - m_timer));
		m_rankSpriteRender.Update();

		if (m_timer > 5.9f)
			m_cursor = 1;
	}
}

void Result::GameOver()
{
	if (m_timer > 1.0f)
		m_timer = -0.5f;

	// -t^2 + 2t
	m_alpha = fabsf(-pow(m_timer, 2.0f) + (2 * m_timer));
	m_alpha = min(m_alpha, 1.0f);
	m_alpha *= 3.0f;
	m_alpha = max(m_alpha, 1.0f);

	//カーソルを点滅させる
	m_cursorSpriteRender.SetPosition(Vector3(-650.0f, 90.0f + (m_cursor * -240.0f), 0.0f));
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_cursorSpriteRender.Update();
}

void Result::Render(RenderContext& rc)
{
	m_backGroundSpriteRender.Draw(rc);
	m_stateSpriteRender.Draw(rc);

	if (!IsEndAnimation()) {
		return;
	}

	for (int i = 0; i < 4; i++) {
		if (m_timer > ENABLE_TIME[i])
			m_messageFontRender[i].Draw(rc);
	}

	if (m_timer >= ENABLE_TIME[4]) {
		m_explainSpriteRender[0].Draw(rc);
		m_scoreFontRender.Draw(rc);
		m_rankSpriteRender.Draw(rc);
		m_cursorSpriteRender.Draw(rc);

		if(m_resultState == enState_GameOver)
			m_failedSpriteRender.Draw(rc);
	}

	m_explainSpriteRender[1].Draw(rc);
}
