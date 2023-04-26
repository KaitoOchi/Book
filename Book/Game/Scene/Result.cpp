#include "stdafx.h"
#include "Result.h"

#include "Fade.h"
#include "Title.h"
#include "Game.h"
#include "GameUI.h"

namespace
{
	const float CAN_INPUT_GAMECLEAR = 5.0f;							//ゲームクリア時の入力可能時間
	const float CAN_INPUT_GAMEOVER = 2.0f;							//ゲームオーバー時の入力可能時間
	const float	ENABLE_TIME[5] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };	//表示可能になる時間
	const char RANK[4] = { 'A', 'B', 'C', 'D' };					//ランク一覧
	const int SCORE_RANK[4] = { 5000, 4000, 3000, 2000 };			//ランクの条件
}

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	switch (m_resultState)
	{
	//ゲームクリアなら
	case enState_GameClear:
		InitGameClear();
		break;

	//ゲームオーバーなら
	case enState_GameOver:
		InitGameOver();
		break;
	}

	//フェードの処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Result::InitGameClear()
{
	int lightNum = 0;

	GameUI* gameUI = FindGO<GameUI>("gameUI");

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
			m_stateSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_stateSpriteRender.SetPosition(objData.position);
			m_stateSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("result_rank") == true) {
			m_explainSpriteRender[0].Init(objData.ddsFilePath, objData.width, objData.height);
			m_explainSpriteRender[0].SetPosition(objData.position);

			m_rankSpriteRender.SetPosition(objData.position);
			m_rankSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("result_text") == true) {
			m_explainSpriteRender[1].Init(objData.ddsFilePath, objData.width, objData.height);
			m_explainSpriteRender[1].SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualObjectName("light") == true) {
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

	//リザルトを保存
	//m_score[0] = gameUI->GetTime();
	m_score[0] = 180;
	m_score[1] = 1;
	m_score[2] = GameManager::GetInstance()->GetSearchNum();
	//m_score[2] = 5;
	m_score[3] = (m_score[0] * 10) +
				 (m_score[1] * 5000) + 
				 (m_score[2] * -500);

	char rank = 'D';
	//スコアからランクを設定
	for(int i = 0; i < 4; i++) {
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
				L"  SUCCESS!"
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

	//待機時間の設定
	m_canInputTime = CAN_INPUT_GAMECLEAR;
}

void Result::InitGameOver()
{
	//背景画像の設定
	m_backGroundSpriteRender.Init("Assets/sprite/UI/gameOver/base.DDS", 1920.0f, 1080.0f);

	//カーソルの設定
	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);

	//タイトル画像の設定
	m_stateSpriteRender.Init("Assets/sprite/UI/gameOver/gameOver.DDS", 516.0f, 138.0f);
	m_stateSpriteRender.SetPosition(Vector3(-550.0f, 400.0f, 0.0f));
	m_stateSpriteRender.Update();

	//詳細画像の設定
	m_explainSpriteRender[0].Init("Assets/sprite/UI/gameOver/retry.DDS", 317.0f, 149.0f);
	m_explainSpriteRender[0].SetPosition(Vector3(-600.0f, 90.0f, 0.0f));
	m_explainSpriteRender[1].Init("Assets/sprite/UI/gameOver/giveup.DDS", 481.0f, 152.0f);
	m_explainSpriteRender[1].SetPosition(Vector3(-600.0f, -150.0f, 0.0f));

	for (int i = 0; i < 2; i++) {
		m_explainSpriteRender[i].SetPosition(Vector3(-600.0f, 90.0f + (-150.0f * i), 0.0f));
		m_explainSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_explainSpriteRender[i].Update();
	}

	//待機時間の設定
	m_canInputTime = CAN_INPUT_GAMEOVER;
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
			//シーン遷移
			if (m_resultState == enState_GameClear) {
				NewGO<Title>(0, "title");
			}
			else if (m_cursor == 0) {
				NewGO<Game>(0, "game");
			}
			else {
				NewGO<Title>(0, "title");
			}
			DeleteGO(this);
		}

		return;
	}
	else {
		Input();
	}

	//時間の処理
	m_timer += g_gameTime->GetFrameDeltaTime();

	switch (m_resultState)
	{
		//ゲームクリアなら
	case enState_GameClear:
		GameClear();
		break;

		//ゲームオーバーなら
	case enState_GameOver:
		GameOver();
		break;
	}
}

void Result::Input()
{
	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA)) {

		//クリアステートで、表示が全て完了していなら
		if (m_resultState == enState_GameClear && m_timer < ENABLE_TIME[4]) {
			//表示をスキップする
			m_timer = ENABLE_TIME[4];
		}
		else {
			//フェードを始める
			m_isWaitFadeOut = true;
			m_fade->StartFadeOut();
		}
	}

	//十字ボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor++;
	}
	else if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor--;
	}
	m_cursor = min(max(m_cursor, 0), 1);
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

	//ゲームクリアなら
	if (m_resultState == enState_GameClear) {

		for (int i = 0; i < 4; i++) {
			if(m_timer > ENABLE_TIME[i])
				m_messageFontRender[i].Draw(rc);
		}

		if (m_timer >= ENABLE_TIME[4]) {
			m_explainSpriteRender[0].Draw(rc);
			m_rankSpriteRender.Draw(rc);
			m_cursorSpriteRender.Draw(rc);
		}
	}
	//ゲームオーバーなら
	else {
		m_cursorSpriteRender.Draw(rc);
		m_explainSpriteRender[0].Draw(rc);
	}
	m_explainSpriteRender[1].Draw(rc);
}
