#include "stdafx.h"
#include "Title_Setting.h"

#include "GameManager.h"
#include "Fade.h"
#include "Title.h"

namespace
{
	const int CURSOR_VERTICAL_MAX = 2;							//縦カーソル最大値
	const int CURSOR_HORIZONTAL_MAX[3] = { 100, 100, 2 };		//各設定の横カーソル最大値
	const char COMMAND[8] = { 'w','s','s','j','b','7','s','j'};
	const Vector3 CURSOR_POS_VERTICAL[3] = { Vector3(-600.0f, 165.0f , 0.0f),
											Vector3(-600.0f, -40.0f, 0.0f),
											Vector3(-600.0f, -240.0f, 0.0f) };				//縦カーソルの座標
	const Vector3 CURSOR_POS_HORIZONTAL[3] = { Vector3(-120.0f, -240.0f , 0.0f),
											Vector3(155.0f, -240.0f, 0.0f),
											Vector3(425.0f, -240.0f, 0.0f) };				//横カーソルの座標
}

Title_Setting::Title_Setting()
{
	m_sprites.reserve(16);
}

Title_Setting::~Title_Setting()
{
	for (auto& sprites : m_sprites)
	{
		m_sprites.pop_back();
	}
	m_sprites.shrink_to_fit();
}

bool Title_Setting::Start()
{
	//セーブデータのロード
	m_saveData = GameManager::GetInstance()->DataLoad();
	SetDataArray();
	m_cursor_horizontal = m_saveDataArray[m_cursor_vertical];

	//設定画面の設定
	m_settingSpriteRender.Init("Assets/sprite/UI/title/setting_all.DDS", 1920.0f, 1080.0f);
	m_sprites.push_back(&m_settingSpriteRender);

	//設定テキストの設定
	m_settingTextSpriteRender[0].Init("Assets/sprite/UI/setting/BGM_text.DDS", 443.0f, 31.0f);
	m_settingTextSpriteRender[1].Init("Assets/sprite/UI/setting/SE_text.DDS", 443.0f, 31.0f);
	m_settingTextSpriteRender[2].Init("Assets/sprite/UI/setting/FPS_text.DDS", 443.0f, 31.0f);
	for (int i = 0; i < 3; i++) {
		m_settingTextSpriteRender[i].SetPosition(Vector3(0.0f, -400.0f, 0.0f));
		m_settingTextSpriteRender[i].Update();
		m_sprites.push_back(&m_settingTextSpriteRender[i]);
	}

	//カーソル画像の設定
	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);
	m_cursorSpriteRender.SetPosition(CURSOR_POS_VERTICAL[0]);
	m_cursorSpriteRender.Update();
	m_sprites.push_back(&m_cursorSpriteRender);

	//FPSカーソル画像の設定
	m_fpsCursorSpriteRender.Init("Assets/sprite/UI/setting/light.DDS", 991.0f, 932.0f);
	m_fpsCursorSpriteRender.SetPosition(CURSOR_POS_HORIZONTAL[m_saveDataArray[2]]);
	m_fpsCursorSpriteRender.SetScale(Vector3(0.1f, 0.1f, 0.0f));
	m_fpsCursorSpriteRender.Update();
	m_sprites.push_back(&m_fpsCursorSpriteRender);

	//ゲージ画像の設定
	m_gaugeSpriteRender[0].SetPosition(Vector3(-210.2, 166.4f, 0.0f));
	m_gaugeSpriteRender[1].SetPosition(Vector3(-211.3, -33.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Init("Assets/sprite/UI/setting/gauge.DDS", 750.0f, 67.0f, AlphaBlendMode_Trans, 2 + i);
		m_gaugeSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_gaugeSpriteRender[i].Update();
		m_sprites.push_back(&m_gaugeSpriteRender[i]);
	}

	//ボタン画像の設定
	m_buttonSpriteRender[0].Init("Assets/sprite/UI/button/text_Bbutton.DDS", 287.0f, 152.0f);
	m_buttonSpriteRender[1].Init("Assets/sprite/UI/button/text_crosskey.DDS", 198.0f, 133.0f);
	m_buttonSpriteRender[2].Init("Assets/sprite/UI/setting/Button_tryangle.DDS", 303.0f, 132.0f);
	for (int i = 0; i < 3; i++) {
		m_buttonSpriteRender[i].SetPosition(Vector3(-825.0f, -275.0f - (i * 50.0f), 0.0f));
		m_buttonSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_buttonSpriteRender[i].SetScale(Vector3(0.75f, 0.75f, 0.0f));
		m_buttonSpriteRender[1].SetScale(Vector3(1.0f, 1.0f, 0.0f));
		m_buttonSpriteRender[i].Update();
		m_sprites.push_back(&m_buttonSpriteRender[i]);
	}

	m_catSpriteRender.Init("Assets/sprite/UI/setting/cat.DDS", 854.0f, 480.0f);
	m_catSpriteRender.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	m_catSpriteRender.Update();
	m_sprites.push_back(&m_catSpriteRender);

	//パーセント文字の設定
	m_percentFontRender.SetPosition(Vector3(-320.0f, 210.0f, 0.0f));
	m_percentFontRender.SetScale(0.9f);
	m_percentFontRender.SetShadowParam(true, 1.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	SpriteUpdate();

	//フェードの処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Title_Setting::Update()
{
	//フェードの待機時間
	if (m_isWaitState) {
		StateChange();
	}
	else {
		m_title->Animation(m_timer, m_alpha);
		m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));

		Input();
	}
}

void Title_Setting::Input()
{
	//カーソルの移動中は、入力を受け付けない
	if (m_cursorTimer < 1.0f) {
		CursorMove();
		return;
	}

	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA)) {
		Command('j');
	}
	//Bボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonB)) {
		m_timer = 1.0f;
		m_isWaitState = true;

		m_title->IsCanPlaySound(false);
	}

	//上ボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor_vertical--;
		m_isSetFPS = false;
		ValueUpdate(true, false, false);
		m_nextCursor = 1;
		Command('w');
	}
	//下ボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor_vertical++;
		m_isSetFPS = false;
		if (m_cursor_vertical == 2) {
			ValueUpdate(true, false, true);
		}
		else {
			ValueUpdate(true, false, false);
		}
		m_nextCursor = -1;
		Command('s');
	}
	//RB1ボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonRB1)) {
		Command('7');
	}
	//LB1ボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonLB1)) {
		Command('b');
	}
	//SELECTボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonSelect)) {
		ResetSaveData();
	}

	//BGM、SFX設定なら
	if (m_cursor_vertical == 0 || m_cursor_vertical == 1) {
		//左ボタンが押されたら
		if (g_pad[0]->IsPress(enButtonLeft)) {
			m_cursor_horizontal--;
			ValueUpdate(false, true, false);
		}
		//右ボタンが押されたら
		else if (g_pad[0]->IsPress(enButtonRight)) {
			m_cursor_horizontal++;
			ValueUpdate(false, true, false);
		}
	}
	//FPS設定なら
	else {
		//左ボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonLeft)) {
			m_cursor_horizontal--;
			m_isSetFPS = true;
			ValueUpdate(false, false, false);
			m_nextCursor = 1;
		}
		//右ボタンが押されたら
		else if (g_pad[0]->IsTrigger(enButtonRight)) {
			m_cursor_horizontal++;
			m_isSetFPS = true;
			ValueUpdate(false, false, false);
			m_nextCursor = -1;
		}
	}
}

void Title_Setting::CursorMove()
{
	m_cursorTimer += g_gameTime->GetFrameDeltaTime() * 4.0f;

	// -t^2 + 2t
	float rate = ((pow(m_cursorTimer, 2.0f) * -1.0f) + (2.0f * m_cursorTimer));
	rate = min(rate, 1.0f);

	//カーソルを移動
	if (m_isSetFPS) {
		m_cursorPos.Lerp(rate, CURSOR_POS_HORIZONTAL[m_cursor_horizontal + m_nextCursor], CURSOR_POS_HORIZONTAL[m_cursor_horizontal]);
		m_fpsCursorSpriteRender.SetPosition(m_cursorPos);
		m_fpsCursorSpriteRender.Update();
	}
	else{
		m_cursorPos.Lerp(rate, CURSOR_POS_VERTICAL[m_cursor_vertical + m_nextCursor], CURSOR_POS_VERTICAL[m_cursor_vertical]);
		m_cursorSpriteRender.SetPosition(m_cursorPos);
		m_cursorSpriteRender.Update();
	}
}

void Title_Setting::ValueUpdate(const bool vertical, const bool vol, const bool setFPS)
{
	int cursor_v = m_cursor_vertical;
	int cursor_h = m_cursor_horizontal;

	//範囲外にはみ出さないようにする
	m_cursor_vertical = min(max(m_cursor_vertical, 0), CURSOR_VERTICAL_MAX);
	m_cursor_horizontal = min(max(m_cursor_horizontal, 0), CURSOR_HORIZONTAL_MAX[m_cursor_vertical]);

	if ((m_cursor_vertical == cursor_v && m_cursor_horizontal == cursor_h) || setFPS) {
		//音を鳴らす
		m_title->Sound(2);

		if (!vol) {
			m_cursorTimer = 0.0f;
		}
	}

	//今保持している設定の値に移動する
	if (vertical) {
		m_cursor_horizontal = m_saveDataArray[m_cursor_vertical];
	}
	//配列に値を保存する
	else {
		m_saveDataArray[m_cursor_vertical] = m_cursor_horizontal;
		//保存する
		SetSaveData();
		GameManager::GetInstance()->SetVolume();
	}

	//画像の更新処理
	SpriteUpdate();
}

void Title_Setting::Command(char command)
{
	//配列の最後に入力する
	m_commandList[15] = command;

	int j = 0;
	//コマンドが合ってるかどうか判定
	for (int i = sizeof(COMMAND) / sizeof(char); i < 16; i++) {
		//コマンドリストと同じでないなら
		if (m_commandList[i] != COMMAND[j]) {
			break;
		}
		j++;

		//コマンド成功なら
		if (j == sizeof(COMMAND) / sizeof(char)) {
			m_percentFontRender.SetText(L"NEKO NEKO POWER 29");
			m_percentFontRender.SetPosition(Vector3(0.0f, 300.0f, 0.0f));
			m_percentFontRender.SetScale(2.0f);
			m_percentFontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			m_percentFontRender.SetShadowParam(true, 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			m_catSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			m_catSpriteRender.Update();
		}
	}

	//配列を一つ分ずらす
	for (int i = 0; i < 15; i++) {
		m_commandList[i] = m_commandList[i + 1];
	}
}

void Title_Setting::SpriteUpdate()
{
	wchar_t text[255];
	swprintf_s(text, 255, L"%d\n\n\n\n\n\n%d", m_saveDataArray[0], m_saveDataArray[1]);
	m_percentFontRender.SetText(text);

	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.x = 590.0f + (m_saveDataArray[0] * 7.5f);
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = 590.0f + (m_saveDataArray[1] * 7.5f);
	//BGMとSFX音量のゲージを変更
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Update();
	}
}

void Title_Setting::StateChange()
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

		if (m_timer < 0.0f) {
			m_isWaitFadeOut = true;
			m_fade->StartFadeOut();
		}
		else {
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}
	}

	//画像の透明度を変更
	for (auto& sprites : m_sprites)
	{
		sprites->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(m_timer)));
		sprites->Update();
	}
	m_percentFontRender.SetColor(Vector4(m_timer, m_timer, m_timer, m_timer));
	m_percentFontRender.SetShadowParam(true, 1.0f, Vector4(m_timer, m_timer, m_timer, m_timer));
}

void Title_Setting::Render(RenderContext& rc)
{
	//ゲージ画像の描画
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Draw(rc);
	}

	//ボタン画像の描画
	for (int i = 0; i < 3; i++) {
		m_buttonSpriteRender[i].Draw(rc);
	}

	//fpsカーソルの描画
	m_fpsCursorSpriteRender.Draw(rc);

	//ベース画像の描画
	m_settingSpriteRender.Draw(rc);

	//説明画像の描画
	m_settingTextSpriteRender[m_cursor_vertical].Draw(rc);

	//パーセント文字の描画
	m_percentFontRender.Draw(rc);

	//カーソルの描画
	m_cursorSpriteRender.Draw(rc);

	//ねこ画像の描画
	m_catSpriteRender.Draw(rc);
}