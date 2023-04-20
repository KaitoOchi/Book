#include "stdafx.h"
#include "Title.h"

#include "GameManager.h"


namespace
{
	const int CURSOR_VERTICAL_MAX[5] = { 0, 3, 2, 0, 4 };		//各ステートの縦カーソル最大値
	const int CURSOR_HORIZONTAL_MAX[10] = { 0, 10, 10, 2, 2 };	//各設定の横カーソル最大値
}


Title::Title()
{

}

Title::~Title()
{

}

bool Title::Start()
{
	for (int i = 0; i < 10; i++) {
		//BGM音量の画像を設定
		m_bgmSpriteRender[i].Init("Assets/sprite/UI/cautionTimeGauge/gaugeCount.DDS", 83.0f, 100.0f);
		m_bgmSpriteRender[i].SetPosition(Vector3(50.0f * i, 300.0f, 0.0f));
		m_bgmSpriteRender[i].Update();

		//SFX音量の画像を設定
		m_sfxSpriteRender[i].Init("Assets/sprite/UI/cautionTimeGauge/gaugeCount.DDS", 83.0f, 100.0f);
		m_sfxSpriteRender[i].SetPosition(Vector3(50.0f * i, 100.0f, 0.0f));
		m_sfxSpriteRender[i].Update();
	}

	//レベルを構築する。
	//レベルのデータを使用して画像を読み込む。
	m_level2DRender.Init("Assets/level2D/titleLevel.casl", [&](Level2DObjectData& objData) {
		//名前が一致していたら。
		if (objData.EqualObjectName("base") == true) {
			//背景を設定
			m_backGroundSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_backGroundSpriteRender.SetScale(objData.scale);
			m_backGroundSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("title") == true) {
			//タイトルを設定
			m_titleSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_titleSpriteRender.SetPosition(objData.position);
			m_titleSpriteRender.SetScale(objData.scale);
			m_titleSpriteRender.Update();
			return true;
		}
		else if (objData.EqualObjectName("button") == true) {
			//ボタンを設定
			m_pressSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_pressSpriteRender.SetPosition(objData.position);
			m_pressSpriteRender.SetScale(objData.scale);
			return true;
		}
		return false;
	});
	
	//背景の設定
	m_backGroundSpriteRender.Init("Assets/sprite/UI/title/base.DDS", 1920.0f, 1080.0f);

	m_menuFontRender.SetText(L"Game Start\n\nGUIDE\n\nSetting");
	m_menuFontRender.SetPosition(Vector3(-600.0f, 0.0f, 0.0f));

	m_settingFontRender.SetText(L"BGM\n\nSFX\n\nSETTING_A\n\nSETTING_B");
	m_settingFontRender.SetPosition(Vector3(-300.0f, 400.0f, 0.0f));

	m_cursorFontRender.SetText(L">");

	m_debugFontRender.SetPosition(Vector3(500.0f, 200.0f, 0.0f));

	//セーブデータのロード
	m_saveData = GameManager::GetInstance()->DataLoad();

	SetDataArray();

	return true;
}

void Title::Update()
{
	//入力処理
	Input();

	//アニメーション処理
	Animation();

	//ステートの遷移処理
	ManageState();


	wchar_t debugText[255];
	swprintf_s(debugText, 255,
		L"STATE:%d \nCURSOR_V:%d \nCURSOR_H:%d \nSA0:%f \nSA1:%f",
		m_titleState,
		m_cursor_vertical,
		m_cursor_horizontal,
		m_saveDataArray[0],
		m_saveDataArray[1]
	);
	m_debugFontRender.SetText(debugText);
}

void Title::Input()
{
	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_titleState = m_cursor_vertical + 1;
		m_cursor_vertical = 0;
	}
	//Bボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonB))
	{
		//設定画面なら
		if (m_titleState == 4) {
			//保存して閉じる
			SetSaveData();
			SetDataArray();
		}

		if (m_titleState >= 2) {
			m_titleState = 1;
		}
		else {
			m_titleState--;
		}
	}

	//上ボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor_vertical--;
		ValueUpdate(true);
	}
	//下ボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor_vertical++;
		ValueUpdate(true);
	}
	//左ボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonLeft)) {
		m_cursor_horizontal--;
		ValueUpdate(false);
	}
	//右ボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonRight)) {
		m_cursor_horizontal++;
		ValueUpdate(false);
	}

	//範囲外にはみ出さないようにする
	m_cursor_vertical = min(max(m_cursor_vertical, 1), CURSOR_VERTICAL_MAX[m_titleState]);
	m_cursor_horizontal = min(max(m_cursor_horizontal, 0), CURSOR_HORIZONTAL_MAX[m_cursor_vertical]);
}

void Title::ValueUpdate(bool vertical)
{
	//ゲームスタート画面なら
	if (m_titleState == 2) {

	}
	//設定画面なら
	else if (m_titleState == 4) {

		if (vertical) {
			//今保持している設定の値に移動する
			float tmp = m_saveDataArray[m_cursor_vertical - 1] * 10;
			m_cursor_horizontal = (int)tmp;
		}
		else {
			//配列に値を保存する
			m_saveDataArray[m_cursor_vertical - 1] = m_cursor_horizontal / 10.0f;
		}
	}
}

void Title::Animation()
{
	//時間の処理
	m_timer += g_gameTime->GetFrameDeltaTime();
	if (m_timer > 1.0f)
		m_timer = -0.5f;

	// (-2 * t^3) + (3 * t^2)
	m_alpha = fabsf((-2 * pow(m_timer, 3.0f)) + (3 * pow(m_timer, 2.0f)));

	//透明度を変更
	m_cursorFontRender.SetColor(Vector4(m_alpha, m_alpha, m_alpha, m_alpha));
}

void Title::ManageState()
{
	switch (m_titleState)
	{
	//タイトル画面なら
	case 0:
		TitleScreen();
		break;

	//メニュー画面なら
	case 1:
		MenuScreen();
		break;

	//ゲームスタートなら
	case 2:
		break;

	//操作方法画面なら
	case 3:
		break;

	//設定画面なら
	case 4:
		SettingScreen();
		break;
	}
}

void Title::TitleScreen()
{
	m_pressSpriteRender.SetMulColor(Vector4(m_alpha, m_alpha, m_alpha, m_alpha));
	m_pressSpriteRender.Update();
}

void Title::MenuScreen()
{
	m_cursorFontRender.SetPosition(Vector3(-700.0f, 150.0f + (100 * -m_cursor_vertical), 0.0f));
}

void Title::StartScreen()
{

}

void Title::SettingScreen()
{
	m_cursorFontRender.SetPosition(Vector3(-300.0f, 350.0f + (100 * -m_cursor_vertical), 0.0f));
}

void Title::Render(RenderContext &rc)
{
	m_backGroundSpriteRender.Draw(rc);

	switch (m_titleState)
	{
	//タイトル画面なら
	case 0:
		m_pressSpriteRender.Draw(rc);
		m_titleSpriteRender.Draw(rc);
		break;

	//メニュー画面なら
	case 1:
		m_menuFontRender.Draw(rc);
		m_cursorFontRender.Draw(rc);
		break;

	//ゲームスタート画面なら
	case 2:
		break;

	//操作方法画面なら
	case 3:
		break;

	//設定画面なら
	case 4:
		m_settingFontRender.Draw(rc);
		m_cursorFontRender.Draw(rc);

		for (int i = 0; i < m_saveDataArray[0] * 10; i++) {
			m_bgmSpriteRender[i].Draw(rc);
		}

		for (int i = 0; i < m_saveDataArray[1] * 10; i++) {
			m_sfxSpriteRender[i].Draw(rc);
		}
		break;
	}

	m_debugFontRender.Draw(rc);
}